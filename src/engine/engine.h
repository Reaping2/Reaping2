#ifndef INCLUDED_ENGINE_ENGINE_H
#define INCLUDED_ENGINE_ENGINE_H

#include "engine/system.h"

#include <boost/ptr_container/ptr_map.hpp>
#include "platform/singleton.h"
#include "platform/i_platform.h"
#include "system_factory.h"
#include "core/opt.h"

using namespace ::boost::multi_index;
namespace engine {

struct SystemElement
{
    int32_t mId;
    int32_t mOrder;
    bool mEnabled;
    mutable Opt<System> mSystem;
    SystemElement(int32_t id, int32_t order, bool enabled, Opt<System> system);
};

class SystemHolder
{
public:

    struct SystemDefaultOrderer
    { 
        typedef int32_t result_type;
        result_type operator()(const SystemElement& system)const;
    };

    struct SystemIdOrderer
    { 
        typedef int32_t result_type;
        result_type operator()(const SystemElement& system)const;
    };

    struct IsEnabled
    { 
        typedef bool result_type;
        bool operator()(const SystemElement& system)const;
    };

    typedef multi_index_container<
        SystemElement,
        indexed_by<
            ordered_unique<
                SystemHolder::SystemIdOrderer
            >,
            ordered_non_unique<
                composite_key<
                    SystemElement,
                    SystemHolder::IsEnabled,
                    SystemHolder::SystemDefaultOrderer
                >
            >
        >
    > Systems_t;
    Systems_t mSystems;
};

typedef SystemHolder::Systems_t Systems_t;

template<int N>
class SystemsFilter
{
public:
    typedef Systems_t::const_iterator const_iterator;
protected:
    Systems_t::const_iterator mI;
    Systems_t::const_iterator mE;
public:
    SystemsFilter(Systems_t const& systems)
    {
        mI=systems.begin();
        mE=systems.end();
    }
    const_iterator begin()
    {
        return mI;
    }
    const_iterator end()
    {
        return mE;
    }
};

template<>
class SystemsFilter<1>
{
public:
    typedef Systems_t::nth_index<1>::type::const_iterator const_iterator;
protected:
    const_iterator mI;
    const_iterator mE;
public:
    SystemsFilter(Systems_t const& systems)
    {
        boost::tie(mI,mE)=systems.get<1>().equal_range(boost::make_tuple(true));
    }
    const_iterator begin()
    {
        return mI;
    }
    const_iterator end()
    {
        return mE;
    }
};

class SystemEnableModifier
{
public:
    SystemEnableModifier(bool enabled);
    void operator()(SystemElement& system);
protected:
    bool mEnabled;
};

class Engine : public platform::Singleton<Engine>
{
    friend class platform::Singleton<Engine>;
    SystemHolder mSystemHolder;
    SystemFactory& mSystemFactory;
    enum SystemIndex
    {
        All,
        EnabledSystems
    };
public:
    Engine();
    ~Engine();
    void Init();
    void Update( double DeltaTime );
    void AddSystem( int32_t Id );
    void SetEnabled( int32_t Id, bool enabled);
};

} // namespace engine

#endif//INCLUDED_ENGINE_COLLISION_SYSTEM_H