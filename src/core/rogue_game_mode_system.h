#ifndef INCLUDED_ENGINE_ROGUE_GAME_MODE_SYSTEM_H
#define INCLUDED_ENGINE_ROGUE_GAME_MODE_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/start_game_mode_event.h"
#include "core/level_selected_event.h"
#include "map_start_event.h"
#include "map_load_event.h"
#include "platform/i_platform.h"

namespace core {

class RogueGameModeSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(RogueGameModeSystem)
    RogueGameModeSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    class Components : public ComponentHolder
    {
    public:
        void Clear( bool deleteComps = false );
        ComponentHolder::ComponentList_t& GetComponents();
    };
    Scene& mScene;
    AutoReg mOnStartGameMode;
    AutoReg mOnLevelSelected;
    core::ProgramState& mProgramState;
    void OnStartGameMode( core::StartGameModeEvent const& Evt );
    void OnLevelSelected( core::LevelSelectedEvent const& Evt );
    AutoReg mOnMapStart;
    void OnMapStart( core::MapStartEvent const& Evt );
    AutoReg mOnMapLoad;
    void OnMapLoad( core::MapLoadEvent const& Evt );

    template<typename Component_t>
    std::auto_ptr<Component_t> Clone(Opt<Component_t> component) const;
    Components mComponents;
};

template<typename Component_t>
std::auto_ptr<Component_t> RogueGameModeSystem::Clone( Opt<Component_t> component ) const
{
    std::ostringstream oss;
    eos::portable_oarchive oa( oss );
    oa& component;
    std::string comp = oss.str();
    std::istringstream iss( comp );
    eos::portable_iarchive ia( iss );
    Opt<Component_t> cloneComp;
    ia >> cloneComp;
    return std::auto_ptr<Component_t>(cloneComp.Get());
}

} // namespace engine

#endif//INCLUDED_ENGINE_ROGUE_GAME_MODE_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "rogue_game_mode_system"
