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
                std::auto_ptr<CreateActorMessage> createActorMsg(new CreateActorMessage);
                createActorMsg->mActorGUID=player->GetGUID();
                createActorMsg->mActorId=playerAutoId;
                mMessageHolder.AddOutgoingMessage(createActorMsg);

                std::auto_ptr<SetOwnershipMessage> setOwnershipMsg(new SetOwnershipMessage);
                setOwnershipMsg->mActorGUID=player->GetGUID();
                setOwnershipMsg->mClientId=i->mClientId;
                mMessageHolder.AddOutgoingMessage(setOwnershipMsg);

                Opt<IPositionComponent> positionC = player->Get<IPositionComponent>();
                positionC->SetX(dimensions.x + ( rand() % ( int )( 500 * ( dimensions.z - dimensions.x ) ) ) / 500.);
                positionC->SetY(dimensions.y + ( rand() % ( int )( 500 * ( dimensions.w - dimensions.y ) ) ) / 500.);
                std::auto_ptr<PositionMessage> positionMsg(new PositionMessage);
                positionMsg->mX=double(positionC->GetX());
                positionMsg->mY=double(positionC->GetY());
                positionMsg->mOrientation=positionC->GetOrientation();
                positionMsg->mForce=true;
                positionMsg->mActorGUID=player->GetGUID();
                mMessageHolder.AddOutgoingMessage(positionMsg);

                mScene.AddActor(player.release());
            }
        }
    }

} // namespace engine

