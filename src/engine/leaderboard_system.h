#ifndef INCLUDED_ENGINE_LEADERBOARD_SYSTEM_H
#define INCLUDED_ENGINE_LEADERBOARD_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/map_start_event.h"
#include "client_datas_changed_event.h"
#include "core/ctf_program_state.h"
#include "core/program_state.h"

namespace engine {

class LeaderboardSystem : public System
{
public:
    DEFINE_SYSTEM_BASE( LeaderboardSystem )
    LeaderboardSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    core::ProgramState& mProgramState;
    ::ctf::ProgramState& mCtfProgramState;
    ModelValue mLeaderBoardModel;
    std::vector<std::string> mBlueNames;
    std::vector<std::string> mBlueKills;
    std::vector<std::string> mBlueDeaths;
    std::vector<std::string> mBlueScores;
    std::vector<std::string> BlueNames();
    std::vector<std::string> BlueKills();
    std::vector<std::string> BlueDeaths();
    std::vector<std::string> BlueScores();
    std::vector<std::string> mRedNames;
    std::vector<std::string> mRedKills;
    std::vector<std::string> mRedDeaths;
    std::vector<std::string> mRedScores;
    std::vector<std::string> RedNames();
    std::vector<std::string> RedKills();
    std::vector<std::string> RedDeaths();
    std::vector<std::string> RedScores();
    boost::ptr_vector<ModelValue> mKDASModels;
    AutoReg mOnMapStart;
    void OnMapStart( core::MapStartEvent const& Evt );
    AutoReg mOnClientDatasChanged;
    AutoReg mOnScore;
    AutoReg mOnClientScore;
    AutoReg mOnKillScore;
    void UpdateBoard();
    std::string mBlueColor;
    std::string mRedColor;
    std::string const& GetColor( Team::Type team ) const;
};

} // namespace engine

#endif//INCLUDED_ENGINE_LEADERBOARD_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "leaderboard_system" -e "core-mapStart engine-clientDatasChanged"
