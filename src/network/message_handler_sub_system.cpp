#include "network/message_handler_sub_system.h"

namespace network {


MessageHandlerSubSystem::~MessageHandlerSubSystem()
{

}

MessageHandlerSubSystem::MessageHandlerSubSystem()
    : mMessageHolder( MessageHolder::Get() )
    , mScene( Scene::Get() )
    , mProgramState( ProgramState::Get() )
{

}

void MessageHandlerSubSystem::Update( double DeltaTime )
{

}

void DefaultMessageHandlerSubSystem::Init()
{

}

void DefaultMessageHandlerSubSystem::Execute( Message const& message )
{

}


} // namespace engine