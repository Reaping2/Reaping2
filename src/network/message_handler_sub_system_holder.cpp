#include "platform/i_platform.h"
#include "network/message_handler_sub_system_holder.h"
#include "platform/auto_id.h"
#include "move_message.h"
#include "position_message.h"
#include "core/program_state.h"

#include "network/move_message.h"
#include "network/player_controller_message.h"
#include "network/damage_taken_message.h"
#include "network/orientation_message.h"
#include "network/ping_message.h"
#include "network/flash_message.h"
#include "network/soldier_properties_message.h"
#include "network/client_datas_message.h"
#include "network/health_message.h"
#include "network/accuracy_message.h"
#include "network/ctf_client_datas_message.h"
#include "network/set_team_message.h"
#include "network/ctf_score_message.h"
#include "network/show_text_message_message.h"
#include "network/collision_message.h"
#include "network/shot_message.h"
#include "network/kill_score_message.h"
#include "network/client_score_message.h"
#include "network/item_changed_message.h"
#include "network/cloak_changed_message.h"
#include "network/border_message.h"
#include "network/sync_item_message.h"
#include "network/secs_to_revive_message.h"
#include "network/modify_audible_component_message.h"
#include "network/flag_state_changed_message.h"
#include "network/fade_out_message.h"
#include "network/actor_list_message.h"
#include "network/client_id_message.h"
#include "network/my_name_message.h"
#include "network/position_message.h"
#include "network/set_ownership_message.h"
#include "network/lifecycle_message.h"
#include "network/heal_taken_message.h"
#include "network/create_actor_message.h"
#include "network/heading_message.h"
#include "network/pickup_message.h"
#include "network/revive_message.h"

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
        , mScene(Scene::Get())
    {
    }

    void MessageHandlerSubSystemHolder::InitHandlers()
    {
        if( ProgramState::Get().mMode == ProgramState::Local )
        {
            return;
        }
        AddSubSystem(network::ClientIdMessage::GetType_static(),AutoId("client_id_message_handler_sub_system"));
        AddSubSystem(network::MyNameMessage::GetType_static(),AutoId("my_name_message_handler_sub_system"));
        AddSubSystem(network::LifecycleMessage::GetType_static(),AutoId("lifecycle_message_handler_sub_system"));
        AddSubSystem(network::CreateActorMessage::GetType_static(),AutoId("create_actor_message_handler_sub_system"));
        AddSubSystem(network::SetOwnershipMessage::GetType_static(),AutoId("set_ownership_message_handler_sub_system"));
        AddSubSystem(network::PositionMessage::GetType_static(),AutoId("position_message_handler_sub_system"));
        AddSubSystem(network::MoveMessage::GetType_static(),AutoId("move_message_handler_sub_system"));
        AddSubSystem(network::PlayerControllerMessage::GetType_static(),AutoId("player_controller_message_handler_sub_system"));
        AddSubSystem(network::HealTakenMessage::GetType_static(),AutoId("heal_taken_message_handler_sub_system"));
        AddSubSystem(network::DamageTakenMessage::GetType_static(),AutoId("damage_taken_message_handler_sub_system"));
        AddSubSystem(network::OrientationMessage::GetType_static(),AutoId("orientation_message_handler_sub_system"));
        AddSubSystem(network::HeadingMessage::GetType_static(),AutoId("heading_message_handler_sub_system"));
        AddSubSystem(network::PickupMessage::GetType_static(),AutoId("pickup_message_handler_sub_system"));
        AddSubSystem(network::PingMessage::GetType_static(),AutoId("ping_message_handler_sub_system"));
        AddSubSystem(network::ReviveMessage::GetType_static(),AutoId("revive_message_handler_sub_system"));
        AddSubSystem(network::FlashMessage::GetType_static(),AutoId("flash_message_handler_sub_system"));
        AddSubSystem(network::SoldierPropertiesMessage::GetType_static(),AutoId("soldier_properties_message_handler_sub_system"));
        AddSubSystem(network::ClientDatasMessage::GetType_static(),AutoId("client_datas_message_handler_sub_system"));
        AddSubSystem(network::ctf::ClientDatasMessage::GetType_static(),AutoId("ctf_client_datas_message_handler_sub_system"));
        AddSubSystem(network::HealthMessage::GetType_static(),AutoId("health_message_handler_sub_system"));
        AddSubSystem(network::AccuracyMessage::GetType_static(),AutoId("accuracy_message_handler_sub_system"));
        AddSubSystem(network::SetTeamMessage::GetType_static(),AutoId("set_team_message_handler_sub_system"));
        AddSubSystem(network::ctf::CtfScoreMessage::GetType_static(),AutoId("ctf_score_message_handler_sub_system"));
        AddSubSystem(network::KillScoreMessage::GetType_static(),AutoId("kill_score_message_handler_sub_system"));
        AddSubSystem(network::ShowTextMessageMessage::GetType_static(),AutoId("show_text_message_message_handler_sub_system"));
        AddSubSystem(network::CollisionMessage::GetType_static(),AutoId("collision_message_handler_sub_system"));
        AddSubSystem(network::ShotMessage::GetType_static(),AutoId("shot_message_handler_sub_system"));
        AddSubSystem(network::ClientScoreMessage::GetType_static(),AutoId("client_score_message_handler_sub_system"));
        AddSubSystem(network::ItemChangedMessage::GetType_static(),AutoId("item_changed_message_handler_sub_system"));
        AddSubSystem(network::CloakChangedMessage::GetType_static(),AutoId("cloak_changed_message_handler_sub_system"));
        AddSubSystem(network::BorderMessage::GetType_static(),AutoId("border_message_handler_sub_system"));
        AddSubSystem(network::SyncItemMessage::GetType_static(),AutoId("sync_item_message_handler_sub_system"));
        AddSubSystem(network::SecsToReviveMessage::GetType_static(),AutoId("secs_to_revive_message_handler_sub_system"));
        AddSubSystem(network::ModifyAudibleComponentMessage::GetType_static(),AutoId("modify_audible_component_message_handler_sub_system"));
        AddSubSystem(network::FlagStateChangedMessage::GetType_static(),AutoId("flag_state_changed_message_handler_sub_system"));
        AddSubSystem(network::FadeOutMessage::GetType_static(),AutoId("fade_out_message_handler_sub_system"));
        AddSubSystem(network::ActorListMessage::GetType_static(),AutoId("actor_list_message_handler_sub_system"));
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
        mScene.InsertNewActors();
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
