#include "platform/i_platform.h"
#include "network/actor_list_message.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {

ActorListMessageSenderSystem::ActorListMessageSenderSystem()
    : MessageSenderSystem()
{
}


void ActorListMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
}


void ActorListMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
}

ActorListMessageHandlerSubSystem::ActorListMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void ActorListMessageHandlerSubSystem::Init()
{
}

void ActorListMessageHandlerSubSystem::Update( double DeltaTime )
{
    MessageHandlerSubSystem::Update( DeltaTime );
}

void ActorListMessageHandlerSubSystem::Execute( Message const& message )
{
    ActorListMessage const& msg = static_cast<ActorListMessage const&>( message );
    if ( msg.mClientId == mProgramState.mClientId )
    {
        L1( "actorlist arrived for me!" );
        Scene::Get().SetPlayerModels( Opt<Actor>() );
        Scene::Get().ClearActors();
        Scene::Get().SetActors( *msg.mActorList );
        Scene::Get().SetPlayerModels( Scene::Get().GetActor( core::ProgramState::Get().mControlledActorGUID ) );

    }
}

} // namespace network


REAPING2_CLASS_EXPORT_IMPLEMENT( network__ActorListMessage, network::ActorListMessage );
