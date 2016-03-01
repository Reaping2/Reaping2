#include "create_actor_message.h"
#include "platform/i_platform.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

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

BOOST_CLASS_EXPORT_IMPLEMENT(network::CreateActorMessage);
