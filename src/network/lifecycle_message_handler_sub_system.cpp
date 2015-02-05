#include "platform/i_platform.h"
#include "lifecycle_message_handler_sub_system.h"
#include "core/program_state.h"
#include "network/lifecycle_message.h"
#include "core/scene.h"
#include "ui/ui.h"
#include "messsage_holder.h"
namespace network {

    LifecycleMessageHandlerSubSystem::LifecycleMessageHandlerSubSystem()
    {

    }

    void LifecycleMessageHandlerSubSystem::Init()
    {

    }

    void LifecycleMessageHandlerSubSystem::Execute(Message const& message)
    {
        LifecycleMessage const& msg=static_cast<LifecycleMessage const&>(message);
        L1("executing lifecycle: state: %d \n",msg.mState );
        if (core::ProgramState::Get().mMode==core::ProgramState::Client)
        {
            Scene::Get().Load("level1");
            Ui::Get().Load("hud");
        }
        else
        {
            Scene::Get().Load("level1");
            Ui::Get().Load("hud");
            std::auto_ptr<LifecycleMessage> msg(new LifecycleMessage);
            msg->mState=msg->mState;
            MessageHolder::Get().AddOutgoingMessage(std::auto_ptr<Message>(msg.release()));
        }
    }

} // namespace engine

