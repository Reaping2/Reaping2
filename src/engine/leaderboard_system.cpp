#include "platform/i_platform.h"
#include "leaderboard_system.h"

namespace engine {

LeaderboardSystem::LeaderboardSystem()
    : mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
    , mCtfProgramState( ctf::ProgramState::Get() )
    , mLeaderBoardModel( "leaderboard", &RootModel::Get() )
    , mBlueColor("0x00ff00")
    , mRedColor("0x00ff00")
{
}


void LeaderboardSystem::Init()
{
    mOnMapStart=EventServer<core::MapStartEvent>::Get().Subscribe( boost::bind( &LeaderboardSystem::OnMapStart, this, _1 ) );
    mOnClientDatasChanged=EventServer<engine::ClientDatasChangedEvent>::Get().Subscribe( boost::bind( &LeaderboardSystem::OnClientDatasChanged, this, _1 ) );
}


void LeaderboardSystem::Update(double DeltaTime)
{
}

void LeaderboardSystem::OnMapStart(core::MapStartEvent const& Evt)
{
    SetModels();
}

void LeaderboardSystem::OnClientDatasChanged(engine::ClientDatasChangedEvent const& Evt)
{
    SetModels();
}

void LeaderboardSystem::SetModels()
{
    mKDASModels.clear();
    int32_t blueCounter=0;
    int32_t redCounter=0;
    for (::ctf::ProgramState::ClientDatas_t::iterator i=mCtfProgramState.mClientDatas.begin(), e=mCtfProgramState.mClientDatas.end();i!=e;++i)
    {
        ::ctf::ClientData& ctfClientData=*i;
        Opt<core::ClientData> clientData=mProgramState.FindClientDataByClientId(ctfClientData.mClientId);
        if (!clientData.IsValid())
        {
            continue;
        }
        std::string modelString = "team";
        if (ctfClientData.mTeam==Team::Blue)
        {
            ++blueCounter;
            modelString=modelString+"_blue_"+boost::lexical_cast<std::string>(blueCounter)+"_";
        }
        else
        {
            ++redCounter;
            modelString=modelString+"_red_"+boost::lexical_cast<std::string>(redCounter)+"_";
        }

        mKDASModels.push_back(new ModelValue( ModelValue::get_string_t( boost::lambda::bind( &LeaderboardSystem::GetClientName, boost::cref(*clientData.Get() ) ) ),modelString+"name",&mLeaderBoardModel));
        mKDASModels.push_back(new ModelValue( ModelValue::get_int_t( boost::lambda::bind( &LeaderboardSystem::GetKill, boost::cref(*clientData.Get() ) ) ),modelString+"kill",&mLeaderBoardModel));
        mKDASModels.push_back(new ModelValue( ModelValue::get_int_t( boost::lambda::bind( &LeaderboardSystem::GetDeath, boost::cref(*clientData.Get() ) ) ),modelString+"death",&mLeaderBoardModel));
        mKDASModels.push_back(new ModelValue( ModelValue::get_int_t( boost::lambda::bind( &LeaderboardSystem::GetAssist, boost::cref(*clientData.Get() ) ) ),modelString+"assist",&mLeaderBoardModel));
        mKDASModels.push_back(new ModelValue( ModelValue::get_int_t( boost::lambda::bind( &LeaderboardSystem::GetScore, boost::cref(*clientData.Get() ) ) ),modelString+"score",&mLeaderBoardModel));
    }
    mKDASModels.push_back(new ModelValue( ModelValue::get_string_t( boost::lambda::bind( &LeaderboardSystem::GetColor, this, Team::Blue) ),"team_blue_color",&mLeaderBoardModel));
    mKDASModels.push_back(new ModelValue( ModelValue::get_string_t( boost::lambda::bind( &LeaderboardSystem::GetColor, this, Team::Red) ),"team_red_color",&mLeaderBoardModel));
}

std::string const& LeaderboardSystem::GetClientName(core::ClientData const& clientData)
{
    return clientData.mClientName;
}

int32_t const& LeaderboardSystem::GetDeath(core::ClientData const& clientData)
{
    return clientData.mDeath;
}

int32_t const& LeaderboardSystem::GetAssist(core::ClientData const& clientData)
{
    return clientData.mAssist;
}

int32_t const& LeaderboardSystem::GetScore(core::ClientData const& clientData)
{
    return clientData.mScore;
}

int32_t const& LeaderboardSystem::GetKill(core::ClientData const& clientData)
{
    return clientData.mKill;
}

std::string const& LeaderboardSystem::GetColor(Team::Type team) const
{
    return team==Team::Blue?mBlueColor:mRedColor;
}


} // namespace engine

