#include "platform/i_platform.h"
#include "leaderboard_system.h"

namespace engine {

LeaderboardSystem::LeaderboardSystem()
    : mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
    , mCtfProgramState( ctf::ProgramState::Get() )
    , mLeaderBoardModel( "leaderboard", &RootModel::Get() )
    , mBlueColor( "0x00ff00" )
    , mRedColor( "0x00ff00" )
{
}


void LeaderboardSystem::Init()
{
    mOnMapStart = EventServer<core::MapStartEvent>::Get().Subscribe( boost::bind( &LeaderboardSystem::OnMapStart, this, _1 ) );
    mOnClientDatasChanged = EventServer<engine::ClientDatasChangedEvent>::Get().Subscribe( boost::bind( &LeaderboardSystem::OnClientDatasChanged, this, _1 ) );

    mKDASModels.push_back( new ModelValue( ModelValue::get_string_t( boost::lambda::bind( &LeaderboardSystem::GetColor, this, Team::Blue ) ), "team_blue_color", &mLeaderBoardModel ) );
    mKDASModels.push_back( new ModelValue( ModelValue::get_string_t( boost::lambda::bind( &LeaderboardSystem::GetColor, this, Team::Red ) ), "team_red_color", &mLeaderBoardModel ) );

    mKDASModels.push_back( new ModelValue( ModelValue::get_string_vec_t( boost::bind(&LeaderboardSystem::BlueNames, this) ), "team_blue_name", &mLeaderBoardModel ) );
    mKDASModels.push_back( new ModelValue( ModelValue::get_string_vec_t( boost::bind(&LeaderboardSystem::BlueKills, this) ), "team_blue_kill", &mLeaderBoardModel ) );
    mKDASModels.push_back( new ModelValue( ModelValue::get_string_vec_t( boost::bind(&LeaderboardSystem::BlueDeaths, this) ), "team_blue_death", &mLeaderBoardModel ) );
    mKDASModels.push_back( new ModelValue( ModelValue::get_string_vec_t( boost::bind(&LeaderboardSystem::BlueScores, this) ), "team_blue_score", &mLeaderBoardModel ) );
    mKDASModels.push_back( new ModelValue( ModelValue::get_string_vec_t( boost::bind(&LeaderboardSystem::RedNames, this) ), "team_red_name", &mLeaderBoardModel ) );
    mKDASModels.push_back( new ModelValue( ModelValue::get_string_vec_t( boost::bind(&LeaderboardSystem::RedKills, this) ), "team_red_kill", &mLeaderBoardModel ) );
    mKDASModels.push_back( new ModelValue( ModelValue::get_string_vec_t( boost::bind(&LeaderboardSystem::RedDeaths, this) ), "team_red_death", &mLeaderBoardModel ) );
    mKDASModels.push_back( new ModelValue( ModelValue::get_string_vec_t( boost::bind(&LeaderboardSystem::RedScores, this) ), "team_red_score", &mLeaderBoardModel ) );
}


void LeaderboardSystem::Update( double DeltaTime )
{
}

void LeaderboardSystem::OnMapStart( core::MapStartEvent const& Evt )
{
    SetModels();
}

void LeaderboardSystem::OnClientDatasChanged( engine::ClientDatasChangedEvent const& Evt )
{
    SetModels();
}

void LeaderboardSystem::SetModels()
{
    mBlueNames.clear();
    mBlueKills.clear();
    mBlueDeaths.clear();
    mBlueScores.clear();
    mRedNames.clear();
    mRedKills.clear();
    mRedDeaths.clear();
    mRedScores.clear();
    for ( auto it = mCtfProgramState.mClientDatas.begin(), e = mCtfProgramState.mClientDatas.end(); it != e; ++it )
    {
        ::ctf::ClientData& ctfClientData = *it;
        Opt<core::ClientData> clientData = mProgramState.FindClientDataByClientId( ctfClientData.mClientId );
        if ( !clientData.IsValid() )
        {
            continue;
        }
        std::string name = clientData->mClientName;
        std::string kill = std::to_string( clientData->mKill );
        std::string death = std::to_string( clientData->mDeath );
        std::string score = std::to_string( clientData->mScore );
        if ( ctfClientData.mTeam == Team::Blue )
        {
            mBlueNames.push_back( name );
            mBlueKills.push_back( kill );
            mBlueDeaths.push_back( death );
            mBlueScores.push_back( score );
        }
        else
        {
            mRedNames.push_back( name );
            mRedKills.push_back( kill );
            mRedDeaths.push_back( death );
            mRedScores.push_back( score );
        }
    }
}

std::string const& LeaderboardSystem::GetColor( Team::Type team ) const
{
    return team == Team::Blue ? mBlueColor : mRedColor;
}

std::vector<std::string> LeaderboardSystem::BlueNames()
{
    return mBlueNames;
}

std::vector<std::string> LeaderboardSystem::BlueKills()
{
    return mBlueKills;
}

std::vector<std::string> LeaderboardSystem::BlueDeaths()
{
    return mBlueDeaths;
}

std::vector<std::string> LeaderboardSystem::BlueScores()
{
    return mBlueScores;
}

std::vector<std::string> LeaderboardSystem::RedNames()
{
    return mRedNames;
}

std::vector<std::string> LeaderboardSystem::RedKills()
{
    return mRedKills;
}

std::vector<std::string> LeaderboardSystem::RedDeaths()
{
    return mRedDeaths;
}

std::vector<std::string> LeaderboardSystem::RedScores()
{
    return mRedScores;
}


} // namespace engine

