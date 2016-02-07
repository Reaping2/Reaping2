#include "ctf_client_list_displaying_system.h"
#include "platform/event.h"
#include "ctf_client_datas_message.h"
#include "ctf_client_datas_changed_event.h"
#include "team_switch_request_event.h"
#include <algorithm>

// TODO:
// client:
// - stores list: red (stirnglist) and blue(stringlist), name to id mapping
// - sends team switch request if clicked item id is own id: event+message
// - receives team setup
// - implememts getter for team names
//
// server:
// - stores team setup: list <id, name, team>
// - handles team switch request: no team of 0 size is allowed
// - sends team setup

namespace network {

using platform::RootModel;

CtfClientListDisplayingSystem::CtfClientListDisplayingSystem()
    : mCTFModel("ctf_client_list", &RootModel::Get())
    , mBlueNamesModel( (ModelValue::get_string_vec_t) boost::bind( &CtfClientListDisplayingSystem::BlueNames, this) , "rednames", &mCTFModel)
    , mRedNamesModel( (ModelValue::get_string_vec_t) boost::bind( &CtfClientListDisplayingSystem::RedNames, this) , "bluenames", &mCTFModel)
    , mSwitchModel( StringFunc( this, &CtfClientListDisplayingSystem::SwitchTeam ), "switch", &mCTFModel )
{
	mOnCtfClientDatasChanged = platform::EventServer<network::CtfClientDatasChangedEvent>::Get().Subscribe( boost::bind( &CtfClientListDisplayingSystem::OnCtfClientDatasChanged, this, _1 ) );
}

void CtfClientListDisplayingSystem::Init()
{
}

void CtfClientListDisplayingSystem::Update( double DeltaTime )
{
}

std::vector<std::string> CtfClientListDisplayingSystem::BlueNames()
{
	return mBlueNames;
}

std::vector<std::string> CtfClientListDisplayingSystem::RedNames()
{
    return mRedNames;
}

void CtfClientListDisplayingSystem::OnCtfClientDatasChanged( CtfClientDatasChangedEvent const& event )
{
    mCtfClientDatas = event.mCtfClientDatas;
    std::vector<std::string> tmpRedNames, tmpBlueNames;
    for ( size_t i = 0; i < mCtfClientDatas.size(); i++ )
    {
        if ( Team::Red == mCtfClientDatas[i].mTeam )
        {
            tmpRedNames.push_back( mCtfClientDatas[i].mClientName );
        }
        else
        {
            tmpBlueNames.push_back( mCtfClientDatas[i].mClientName );
        }
    }
    mRedNames.swap( tmpRedNames );
    mBlueNames.swap( tmpBlueNames );
}

void CtfClientListDisplayingSystem::SwitchTeam( std::string const & player )
{
    ::ctf::ProgramState::ClientDatas_t::iterator it = std::find_if( mCtfClientDatas.begin(), mCtfClientDatas.end(),
            boost::bind<bool>([]( ::ctf::ClientData const& d, std::string const& s ){ return d.mClientName == s; },_1, player ) );
    if ( mCtfClientDatas.end() != it )
    {
        ::ctf::ClientData clientData = *it;
        TeamSwitchRequestEvent event;
        event.mClientId = (*it).mClientId;
        EventServer<TeamSwitchRequestEvent>::Get().SendEvent(event);
    }
}

}
