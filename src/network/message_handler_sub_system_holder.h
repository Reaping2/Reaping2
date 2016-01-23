#ifndef INCLUDED_NETWORK_MESSAGE_HANDLER_SUB_SYSTEM_HOLDER_H
#define INCLUDED_NETWORK_MESSAGE_HANDLER_SUB_SYSTEM_HOLDER_H

#include "network/message_handler_sub_system.h"

#include <boost/ptr_container/ptr_map.hpp>
#include "platform/singleton.h"
#include "platform/i_platform.h"
#include "core/opt.h"
#include "engine/sub_system_factory.h"
#include "message_handler_sub_system_factory.h"
#include "engine/system.h"
#include "messsage_holder.h"

using namespace ::boost::multi_index;
namespace network {

    struct SubSystemElement
    {
        int32_t mId;
        int32_t mBindedId;
        int32_t mOrder;
        bool mEnabled;
        mutable Opt<MessageHandlerSubSystem> mSystem;
        SubSystemElement(int32_t id, int32_t mBindedId, int32_t order, bool enabled, Opt<MessageHandlerSubSystem> system);
    };


    class MessageHandlerSubSystemHolder : public engine::System
    {
    public:
        DEFINE_SYSTEM_BASE(MessageHandlerSubSystemHolder)

        void AddSubSystem( int32_t BindId, int32_t Id );
        Opt<MessageHandlerSubSystem> GetSubSystem( int32_t BindId ) const;
        Opt<MessageHandlerSubSystem> GetSubSystem( int32_t BindId );
        void SetEnabled( int32_t BindId, bool enabled );
        struct SubSystemDefaultOrderer
        { 
            typedef int32_t result_type;
            result_type operator()(const SubSystemElement& system)const;
        };

        struct SubSystemIdOrderer
        { 
            typedef int32_t result_type;
            result_type operator()(const SubSystemElement& system)const;
        };

        struct SubSystemBindIdOrderer
        { 
            typedef int32_t result_type;
            result_type operator()(const SubSystemElement& system)const;
        };

        struct IsEnabled
        { 
            typedef bool result_type;
            bool operator()(const SubSystemElement& system)const;
        };

        typedef multi_index_container<
            SubSystemElement,
            indexed_by<
                ordered_unique<
                    MessageHandlerSubSystemHolder::SubSystemIdOrderer
                >,
                ordered_unique<
                    MessageHandlerSubSystemHolder::SubSystemBindIdOrderer
                >,
                ordered_non_unique<
                    composite_key<
                        SubSystemElement,
                        MessageHandlerSubSystemHolder::IsEnabled,
                        MessageHandlerSubSystemHolder::SubSystemDefaultOrderer
                    >
                >
            >
        > SubSystems_t;
        enum SubSystemIndex
        {
            AllById,
            AllByBindId,
            EnabledSubSystems
        };
        typedef SubSystems_t::nth_index<MessageHandlerSubSystemHolder::AllByBindId>::type BindIds_t;
        typedef SubSystems_t::nth_index<MessageHandlerSubSystemHolder::EnabledSubSystems>::type EnabledSubSystems_t;
        MessageHandlerSubSystemHolder();
        virtual ~MessageHandlerSubSystemHolder();
        virtual void Init();
        virtual void Update( double DeltaTime );
    protected:
        SubSystems_t mSubSystems;
        MessageHandlerSubSystemFactory& mSubSystemFactory;
        MessageHolder& mMessageHolder;
        Scene& mScene;
    };

    typedef MessageHandlerSubSystemHolder::SubSystems_t SubSystems_t;

    template<int N>
    class SubSystemsFilter
    {
    public:
        typedef SubSystems_t::const_iterator const_iterator;
    protected:
        SubSystems_t::const_iterator mI;
        SubSystems_t::const_iterator mE;
    public:
        SubSystemsFilter(SubSystems_t const& systems)
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
    class SubSystemsFilter<1>
    {
    public:
        typedef SubSystems_t::nth_index<1>::type BindId_t;
        typedef SubSystems_t::nth_index<1>::type::const_iterator const_iterator;
    protected:
        BindId_t& mBindIds;
    public:
        SubSystemsFilter(SubSystems_t& systems)
            : mBindIds(systems.get<1>())
        {
        }
        const_iterator begin()
        {
            return mBindIds.begin();
        }
        const_iterator end()
        {
            return mBindIds.end();
        }
    };

    template<>
    class SubSystemsFilter<2>
    {
    public:
        typedef SubSystems_t::nth_index<2>::type::const_iterator const_iterator;
    protected:
        const_iterator mI;
        const_iterator mE;
    public:
        SubSystemsFilter(SubSystems_t const& systems)
        {
            boost::tie(mI,mE)=systems.get<2>().equal_range(boost::make_tuple(true));
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

    class SubSystemEnableModifier
    {
    public:
        SubSystemEnableModifier(bool enabled);
        void operator()(SubSystemElement& system);
    protected:
        bool mEnabled;
    };

} // namespace network

#endif//INCLUDED_NETWORK_MESSAGE_HANDLER_SUB_SYSTEM_HOLDER_H