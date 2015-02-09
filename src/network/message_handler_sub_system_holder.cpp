#include "platform/i_platform.h"
#include "network/message_handler_sub_system_holder.h"
#include "platform/auto_id.h"
#include "move_message.h"
#include "position_message.h"

using platform::AutoId;
namespace network {
    SubSystemElement::SubSystemElement(int32_t id, int32_t bindedId, int32_t order, bool enabled, Opt<MessageHandlerSubSystem> system)
        : mId(id)
        , mBindedId(bindedId)
        , mOrder(order)
        , mEnabled(enabled)
        , mSystem(system)
    {

    }


    int32_t MessageHandlerSubSystemHolder::SubSystemDefaultOrderer::operator()(const SubSystemElement& system) const
    {
        return system.mOrder;
    }


    int32_t MessageHandlerSubSystemHolder::SubSystemIdOrderer::operator()(const SubSystemElement& system) const
    {
        return system.mId;
    }


    int32_t MessageHandlerSubSystemHolder::SubSystemBindIdOrderer::operator()(const SubSystemElement& system) const
    {
        return system.mBindedId;
    }


    bool MessageHandlerSubSystemHolder::IsEnabled::operator()(const SubSystemElement& system) const
    {
        return system.mEnabled;
    }


    void MessageHandlerSubSystemHolder::AddSubSystem(int32_t BindId, int32_t Id)
    {
        std::auto_ptr<MessageHandlerSubSystem> sys = mSubSystemFactory(Id);
        int32_t typ = sys->GetType();
        mSubSystems.insert(SubSystemElement(typ,BindId,mSubSystems.size(),true,Opt<MessageHandlerSubSystem>(sys.release())));
    }

    Opt<MessageHandlerSubSystem> MessageHandlerSubSystemHolder::GetSubSystem(int32_t BindId) const
    {
        BindIds_t const& BindIdSubSystens=mSubSystems.get<MessageHandlerSubSystemHolder::AllByBindId>();
        BindIds_t::const_iterator it=
            BindIdSubSystens.find(BindId);
        return Opt<MessageHandlerSubSystem>(const_cast<MessageHandlerSubSystem*>(
            it == BindIdSubSystens.end()?NULL:it->mSystem.Get())); 
    }

    Opt<MessageHandlerSubSystem> MessageHandlerSubSystemHolder::GetSubSystem(int32_t BindId)
    {
        return ( ( const MessageHandlerSubSystemHolder* )this )->GetSubSystem(BindId);
    }

    void MessageHandlerSubSystemHolder::SetEnabled(int32_t BindId, bool enabled)
    {
        BindIds_t& BindIdSubSystens=mSubSystems.get<MessageHandlerSubSystemHolder::AllByBindId>();
        BindIds_t::iterator it=
            BindIdSubSystens.find(BindId);
        if (it!=BindIdSubSystens.end())
        {
            BindIdSubSystens.modify(it,SubSystemEnableModifier(enabled));
        }
    }

    MessageHandlerSubSystemHolder::MessageHandlerSubSystemHolder()
        : mSubSystemFactory(MessageHandlerSubSystemFactory::Get())
        , mMessageHolder(MessageHolder::Get())
    {
    }

    MessageHandlerSubSystemHolder::~MessageHandlerSubSystemHolder()
    {
        for( SubSystems_t::iterator it = mSubSystems.begin(), e = mSubSystems.end(); it != e; ++it )
        {
            delete it->mSystem.Get();
        }
        mSubSystems.clear();
    }

    void MessageHandlerSubSystemHolder::Init()
    {
        for( SubSystems_t::iterator it = mSubSystems.begin(), e = mSubSystems.end(); it != e; ++it )
        {
            it->mSystem->Init();
        }
    }

    void MessageHandlerSubSystemHolder::Update(double DeltaTime)
    {
        for(MessageList::Messages_t::iterator i=mMessageHolder.GetIncomingMessages().mMessages.begin(), e=mMessageHolder.GetIncomingMessages().mMessages.end();i!=e;++i)
        {
            Opt<MessageHandlerSubSystem> messageHandlerSS= GetSubSystem(i->GetType());
            if (messageHandlerSS.IsValid())
            {
                messageHandlerSS->Execute(*i);
            }
            else
            {
                L1("cannot find subsystem for type! %d",i->GetType());
            }
        }
        for( SubSystems_t::iterator it = mSubSystems.begin(), e = mSubSystems.end(); it != e; ++it )
        {
            it->mSystem->Update(DeltaTime);
        }
        mMessageHolder.ClearIncomingMessages();
    }


    SubSystemEnableModifier::SubSystemEnableModifier(bool enabled)
        : mEnabled(enabled)
    {

    }

    void SubSystemEnableModifier::operator()(SubSystemElement& system)
    {
        system.mEnabled=mEnabled;
    }

} // namespace engine
