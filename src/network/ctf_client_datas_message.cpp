#include "platform/i_platform.h"
#include "network/ctf_client_datas_message.h"
#include "core/ctf_program_state.h"
#include "engine/client_datas_changed_event.h"
#include "platform/event.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {
namespace ctf {

ClientDatasMessageSenderSystem::ClientDatasMessageSenderSystem()
    : MessageSenderSystem()
{
    mOnCtfClientDatasChanged = platform::EventServer<CtfClientDatasChangedEvent>::Get().Subscribe( boost::bind( &ClientDatasMessageSenderSystem::OnCtfClientDatasChangedEvent, this, _1 ) );
}


void ClientDatasMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
}


void ClientDatasMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
}

void ClientDatasMessageSenderSystem::OnCtfClientDatasChangedEvent( CtfClientDatasChangedEvent const& event )
{
    if ( mProgramState.mMode == ProgramState::Server )
    {
        std::auto_ptr<ctf::ClientDatasMessage> message( new ctf::ClientDatasMessage );
        message->mClientDatas = event.mCtfClientDatas;
        mMessageHolder.AddOutgoingMessage( message );
    }
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
    L2( "executing ctf::ClientDatasMessageHandlerSubSystem from id: %d \n", msg.mSenderId );
    ::ctf::ProgramState& ctfProgramState =::ctf::ProgramState::Get();
    ctfProgramState.mClientDatas = msg.mClientDatas;
    for ( ::ctf::ProgramState::ClientDatas_t::iterator i = ctfProgramState.mClientDatas.begin(), e = ctfProgramState.mClientDatas.end(); i != e; ++i )
    {
        L1( "**** ctf arrived. **** from id: %d \n", i->mClientId );
        L1( "   team:%d\n", i->mTeam );
    }
    if ( mProgramState.mMode == ProgramState::Client )
    {
        CtfClientDatasChangedEvent event;
        event.mCtfClientDatas = msg.mClientDatas;
        EventServer<CtfClientDatasChangedEvent>::Get().SendEvent( event );
    }
}

} // namespace ctf
} // namespace network


REAPING2_CLASS_EXPORT_IMPLEMENT( network__ctf__ClientDatasMessage, network::ctf::ClientDatasMessage );
