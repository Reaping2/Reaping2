#include "waiting_start_system.h"
#include "platform/event.h"
#include "load_clientlist_event.h"
#include "ui/ui.h"
#include <boost/bind.hpp>

namespace network {
using platform::RootModel;

WaitingStartSystem::WaitingStartSystem()
    : mWaitModel("waiting_system",&RootModel::Get())
{
    mOnLoadClientlistEvent = platform::EventServer<network::LoadClientlistEvent>::Get().Subscribe( boost::bind( &WaitingStartSystem::OnLoadClientlistEvent, this, _1 ) );
}

void WaitingStartSystem::Init()
{
}

void WaitingStartSystem::Update( double DeltaTime )
{
}

void WaitingStartSystem::OnLoadClientlistEvent( LoadClientlistEvent const& event )
{
    if ( event.mGameMode == "ffa" )
    {
        Ui::Get().Load("ffa_client_list");
    }
    else if ( event.mGameMode == "ctf" )
    {
        Ui::Get().Load("ctf_client_list");
    }
    else
    {
    }
}

}
