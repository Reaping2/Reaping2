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

        if (mProgramState.mMode == ProgramState::Server
            || (mProgramState.mMode == ProgramState::Client&&msg.mActorGUID != mProgramState.mControlledActorGUID))
        {
            Opt<Actor> actor = mScene.GetActor(msg.mActorGUID);
            if (!actor.IsValid())
            {
                L1("cannot find actor with GUID: (that is not possible) %d \n", msg.mActorGUID);
                return;
            }

            Opt<IMoveComponent> moveC(actor->Get<IMoveComponent>());
            if (!moveC.IsValid())
            {
                L1("move is called on an actor that has no move_component \n");
                return;
            }
            moveC->SetHeadingModifier(msg.mHeadingModifier / PRECISION);
            moveC->GetSpeed().mBase.Set(msg.mSpeed / PRECISION);
            moveC->GetSpeed().mPercent.Set(msg.mPercent / PRECISION);
            moveC->GetSpeed().mFlat.Set(msg.mFlat / PRECISION);
            moveC->SetMoving(msg.mMoving);
            moveC->SetRooted(msg.mRooted);
        }
    }

} // namespace network

