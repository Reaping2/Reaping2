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
    bool mIsRunning;
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
    template<typename System_t>
    Opt<System_t> GetSystem() const;
    template<typename System_t>
    Opt<System_t> GetSystem();
    template<typename System_t>
    void SetEnabled(bool enabled);
    void OnPhaseChangedEvent( PhaseChangedEvent const& Evt );
};

template<typename System_t>
Opt<System_t> Engine::GetSystem() const
{
    Systems_t::const_iterator i = mSystemHolder.mSystems.find( System_t::GetType_static() );
    return Opt<System_t>(static_cast<System_t*>(const_cast<System*>(
        i == mSystemHolder.mSystems.end()?NULL:i->mSystem.Get()))); 
}

template<typename System_t>
Opt<System_t> Engine::GetSystem()
{
    return ( ( const Engine* )this )->GetSystem<System_t>();
}
template<typename System_t>
void Engine::SetEnabled(bool enabled)
{
    Systems_t::iterator it = mSystemHolder.mSystems.find(System_t::GetType_static());
    if (it!=mSystemHolder.mSystems.end())
    {
        mSystemHolder.mSystems.modify(it,SystemEnableModifier(enabled));
    }
}

} // namespace engine

#endif//INCLUDED_ENGINE_ENGINE_H