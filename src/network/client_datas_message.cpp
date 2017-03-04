#include "platform/i_platform.h"
#include "network/client_datas_message.h"
#include "core/program_state.h"
#include "platform/event.h"
#include "engine/client_datas_changed_event.h"
#include "network/client_list_changed_event.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>


namespace network {

ClientDatasMessageSenderSystem::ClientDatasMessageSenderSystem()
    : MessageSenderSystem()
{
}


void ClientDatasMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
}


void ClientDatasMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
}

ClientDatasMessageHandlerSubSystem::ClientDatasMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void ClientDatasMessageHandlerSubSystem::Init()
{
}

void ClientDatasMessageHandlerSubSystem::Execute( Message const& message )
{
    ClientDatasMessage const& msg = static_cast<ClientDatasMessage const&>( message );
    L2( "executing ClientDatasMessageHandlerSubSystem from id: %d \n", msg.mSenderId );
    mProgramState.mClientDatas = msg.mClientDatas;
    EventServer<engine::ClientDatasChangedEvent>::Get().SendEvent( engine::ClientDatasChangedEvent() );
    for ( core::ProgramState::ClientDatas_t::iterator i = mProgramState.mClientDatas.begin(), e = mProgramState.mClientDatas.end(); i != e; ++i )
    {
        L1( "**** %s properties arrived. **** from id: %d \n", i->mClientName.c_str(), msg.mSenderId );
        L1( "   MoveSpeed:%d\n   Health:%d\n   Accuracy:%d\n", i->mSoldierProperties.mMoveSpeed, i->mSoldierProperties.mHealth, i->mSoldierProperties.mAccuracy );
    }
    EventServer<ClientListChangedEvent>::Get().SendEvent( mProgramState.mClientDatas );
}


} // namespace network

REAPING2_CLASS_EXPORT_IMPLEMENT( network__ClientDatasMessage, network::ClientDatasMessage );

