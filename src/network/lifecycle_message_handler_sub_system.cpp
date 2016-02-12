#include "platform/i_platform.h"
#include "lifecycle_message_handler_sub_system.h"
#include "core/program_state.h"
#include "network/lifecycle_message.h"
#include "core/scene.h"
#include "ui/ui.h"
#include "messsage_holder.h"
#include "create_actor_message.h"
#include "core/actor_factory.h"
#include "core/i_position_component.h"
#include "set_ownership_message.h"
#include "position_message.h"
#include "core/program_state.h"
#include "core/i_inventory_component.h"
#include "core/i_move_component.h"
#include "core/buffs/i_buff_holder_component.h"
#include "core/buffs/buff_factory.h"
#include "core/buffs/move_speed_buff.h"
#include "client_datas_message.h"
#include "engine/soldier_spawn_system.h"
#include "core/start_game_mode_event.h"
#include "engine/engine.h"
#include "main/window.h"

namespace network {

    LifecycleMessageHandlerSubSystem::LifecycleMessageHandlerSubSystem()
        : MessageHandlerSubSystem()
    {

    }

    void LifecycleMessageHandlerSubSystem::Init()
    {

    }

    void LifecycleMessageHandlerSubSystem::Execute(Message const& message)
    {
        LifecycleMessage const& msg=static_cast<LifecycleMessage const&>(message);
        L1("executing lifecycle: state: %d \n",msg.mState );
        if (mProgramState.mMode==ProgramState::Client
            &&(msg.mClientId==-1||msg.mClientId==mProgramState.mClientId))
        {
            if (msg.mState==LifecycleMessage::Start)
            {
                EventServer<core::StartGameModeEvent>::Get().SendEvent( core::StartGameModeEvent( msg.mGameMode ));
            }
            else if (msg.mState==LifecycleMessage::WaitingForHost)
            {
                Ui::Get().Load("waiting_start");
            }
            else if (msg.mState==LifecycleMessage::SoldierProperties)
            {
                Ui::Get().Load("soldier_properties");
            }
            else if (msg.mState==LifecycleMessage::AlreadyConnected)
            {
                L1("\n\n\n\nAlready connected. If you lost connection please try reconnecting later! *** One Love!\n");
                engine::Engine::Get().GetSystem<engine::WindowSystem>()->Close();
            }
        }
        else if (mProgramState.mMode==ProgramState::Server)
        {
            if (msg.mState==LifecycleMessage::Start)
            {
                mScene.SelectLevel(msg.mSelectedLevel);
                EventServer<core::StartGameModeEvent>::Get().SendEvent( core::StartGameModeEvent( msg.mGameMode ));
                std::auto_ptr<network::LifecycleMessage> lifecycleMsg(new network::LifecycleMessage);
                lifecycleMsg->mState=msg.mState;
                lifecycleMsg->mGameMode=msg.mGameMode;
                mMessageHolder.AddOutgoingMessage(lifecycleMsg);
                std::auto_ptr<network::ClientDatasMessage> clientDatasMsg(new network::ClientDatasMessage);
                clientDatasMsg->mClientDatas=mProgramState.mClientDatas;
                mMessageHolder.AddOutgoingMessage(clientDatasMsg);
            }
        }
    }

} // namespace engine

