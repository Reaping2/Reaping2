#include "create_actor_message.h"
#include "platform/i_platform.h"
namespace network{

CreateActorMessage::CreateActorMessage(Opt<Actor> actor) 
    : mActorGUID(-1)
    , mState(ActorEvent::Added)
    , mActor()
{
    std::ostringstream oss;
    eos::portable_oarchive oa(oss);
    oa & actor;
    mActor = oss.str();
}
} // namespace network
