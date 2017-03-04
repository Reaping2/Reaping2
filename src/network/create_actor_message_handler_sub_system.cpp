#include "platform/i_platform.h"
#include "network/create_actor_message_handler_sub_system.h"
#include "create_actor_message.h"
#include "core/shot_collision_component.h"
#include "core/i_renderable_component.h"
namespace network {

CreateActorMessageHandlerSubSystem::CreateActorMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
    , mActorFactory( ActorFactory::Get() )
{

}

void CreateActorMessageHandlerSubSystem::Init()
{

}

void CreateActorMessageHandlerSubSystem::Execute( Message const& message )
{
    CreateActorMessage const& msg = static_cast<CreateActorMessage const&>( message );
    if ( msg.mState == ActorEvent::Added )
    {
        std::istringstream iss( msg.mActor );
        eos::portable_iarchive ia( iss );
        Opt<Actor> actor;
        ia >> actor;
        L2( "createactormessage add executed with (GUID:%d)\n", actor->GetGUID() );
        mScene.AddActor( actor.Get() );
    }
    else if ( msg.mState == ActorEvent::Removed )
    {
        L2( "createactormessage remove with (GUID:%d)\n", msg.mActorGUID );
        mScene.RemoveActor( msg.mActorGUID );
    }
}

} // namespace engine

