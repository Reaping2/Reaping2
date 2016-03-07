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
    boost::ptr_vector<ModelValue> mKDASModels;
    AutoReg mOnMapStart;
    void OnMapStart( core::MapStartEvent const& Evt );
    AutoReg mOnClientDatasChanged;
    void OnClientDatasChanged( engine::ClientDatasChangedEvent const& Evt );
    void SetModels();
    static std::string const& GetClientName( core::ClientData const& clientData );
    static int32_t const& GetKill( core::ClientData const& clientData );
    static int32_t const& GetDeath( core::ClientData const& clientData );
    static int32_t const& GetAssist( core::ClientData const& clientData );
    static int32_t const& GetScore( core::ClientData const& clientData );
    std::string mBlueColor;
    std::string mRedColor;
    std::string const& GetColor( Team::Type team ) const;
};

} // namespace engine

#endif//INCLUDED_ENGINE_LEADERBOARD_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "leaderboard_system" -e "core-mapStart engine-clientDatasChanged"
