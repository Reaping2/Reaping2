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
        if (mProgramState.mMode==ProgramState::Client)
        {
            mScene.Load("level1");
            Ui::Get().Load("hud");
        }
        else
        {
            mScene.Load("level1");
            Ui::Get().Load("hud");
            std::auto_ptr<LifecycleMessage> lifecycleMsg(new LifecycleMessage);
            lifecycleMsg->mState=msg.mState;
            mMessageHolder.AddOutgoingMessage(lifecycleMsg);
            std::auto_ptr<ClientDatasMessage> clientDatasMsg(new ClientDatasMessage);
            clientDatasMsg->mClientDatas=mProgramState.mClientDatas;
            mMessageHolder.AddOutgoingMessage(clientDatasMsg);

            ActorFactory& actorFactory=ActorFactory::Get();
            int32_t playerAutoId=AutoId("player");
            glm::vec4 const& dimensions=mScene.GetDimensions();
            Opt<engine::SoldierSpawnSystem> soldierSpawnS(engine::SoldierSpawnSystem::Get());
            for (core::ProgramState::ClientDatas_t::iterator i=mProgramState.mClientDatas.begin(), e=mProgramState.mClientDatas.end();i!=e;++i)
            {
                std::auto_ptr<Actor> player(soldierSpawnS->Spawn(*i));
                mScene.AddActor(player.release());
            }
        }
    }

} // namespace engine

