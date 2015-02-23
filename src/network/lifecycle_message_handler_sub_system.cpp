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
            lifecycleMsg->mState=lifecycleMsg->mState;
            mMessageHolder.AddOutgoingMessage(lifecycleMsg);

            ActorFactory& actorFactory=ActorFactory::Get();
            int32_t playerAutoId=AutoId("player");
            glm::vec4 const& dimensions=mScene.GetDimensions();

            for (core::ProgramState::ClientDatas_t::iterator i=mProgramState.mClientDatas.begin(), e=mProgramState.mClientDatas.end();i!=e;++i)
            {
                std::auto_ptr<Actor> player(actorFactory(playerAutoId));
                Opt<IPositionComponent> positionC = player->Get<IPositionComponent>();
                positionC->SetX((dimensions.x + ( rand() % ( int )( ( dimensions.z - dimensions.x ) ) )) );
                positionC->SetY((dimensions.y + ( rand() % ( int )( ( dimensions.w - dimensions.y ) ) )) );

                //TODO: temporary till normal inventory sync 
                Opt<IInventoryComponent> inventoryC = player->Get<IInventoryComponent>();
                if (inventoryC.IsValid())
                {
                    inventoryC->SetSelectedWeapon(AutoId( "plasma_gun" ));
                }
                AddNewPlayer(*i,player);
            }
        }
    }

    void LifecycleMessageHandlerSubSystem::AddNewPlayer(core::ClientData& clientDataToSet, std::auto_ptr<Actor> player)
    {
        std::auto_ptr<SetOwnershipMessage> setOwnershipMsg(new SetOwnershipMessage);
        setOwnershipMsg->mActorGUID=player->GetGUID();
        setOwnershipMsg->mClientId=clientDataToSet.mClientId;
        MessageHolder::Get().AddOutgoingMessage(setOwnershipMsg);
        clientDataToSet.mClientActor=Opt<Actor>(player.get());
        L2("player added clientId:%d clientName:%s actorId:%d\n",clientDataToSet.mClientId,clientDataToSet.mClientName.c_str(),clientDataToSet.mClientActor->GetGUID());
        Scene::Get().AddActor(player.release());
    }

} // namespace engine

