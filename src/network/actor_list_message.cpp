#include "platform/i_platform.h"
#include "network/actor_list_message.h"
#include "core/map_start_event.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {


ActorListMessage::ActorListMessage( ActorList_t* actorList /*= nullptr */ ) 
    : mActorList()
    , mClientId( -1 )
{
    if (actorList != nullptr)
    {
        std::ostringstream oss;
        eos::portable_oarchive oa( oss );
        oa& actorList;
        mActorList = oss.str();
    }
}



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
    if (msg.mClientId == mProgramState.mClientId || msg.mClientId == -1)
    {
        L1( "actorlist arrived for me!\n" );
        Scene::Get().SetPlayerModels( Opt<Actor>() );
        Scene::Get().ClearActors();
        if (!msg.mActorList.empty())
        {
            std::istringstream iss( msg.mActorList );
            eos::portable_iarchive ia( iss );
            ActorList_t * actorList;
            ia >> actorList;
            Scene::Get().SetActors( *actorList );
        }
        Scene::Get().SetPlayerModels( Scene::Get().GetActor( core::ProgramState::Get().mControlledActorGUID ) );
        EventServer<core::MapStartEvent>::Get().SendEvent( core::MapStartEvent( core::MapStartEvent::Ready ) );
    }
}

} // namespace network


REAPING2_CLASS_EXPORT_IMPLEMENT( network__ActorListMessage, network::ActorListMessage );
