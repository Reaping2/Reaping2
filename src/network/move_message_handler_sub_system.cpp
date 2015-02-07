#include "platform/i_platform.h"
#include "network/move_message_handler_sub_system.h"
#include "core/opt.h"
#include "move_message.h"
#include "core/i_move_component.h"
namespace network {

    MoveMessageHandlerSubSystem::MoveMessageHandlerSubSystem()
        : MessageHandlerSubSystem()
    {

    }

    void MoveMessageHandlerSubSystem::Init()
    {

    }

    void MoveMessageHandlerSubSystem::Execute(Message const& message)
    {
        MoveMessage const& msg=static_cast<MoveMessage const&>(message);
//        L1("executing position: %d \n",msg.mSenderId );
//         if(msg.mActorGUID==mProgramState.mControlledActorGUID
//             && !msg.mForce) //TODO: playercontroller states will be synced
//         {
//             return;
//         }

        Opt<Actor> actor=mScene.GetActor(msg.mActorGUID);
        if (!actor.IsValid())
        {
            L1("cannot find actor with GUID: (that is not possible) %d \n",msg.mActorGUID );
            return;
        }

        Opt<IMoveComponent> moveC(actor->Get<IMoveComponent>());
        if (!moveC.IsValid())
        {
            L1("move is called on an actor that has no move_component \n" );
            return;
        }
        moveC->SetHeading(msg.mHeading);
        moveC->SetSpeed(msg.mSpeed);
        moveC->SetSpeedX(msg.mSpeedX);
        moveC->SetSpeedY(msg.mSpeedY);
    }

} // namespace network

