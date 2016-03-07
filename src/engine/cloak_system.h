#ifndef INCLUDED_ENGINE_CLOAK_SYSTEM_H
#define INCLUDED_ENGINE_CLOAK_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/program_state.h"

namespace engine {

class CloakSystem : public System
{
public:
    DEFINE_SYSTEM_BASE( CloakSystem )
    CloakSystem();
    enum CloakState
    {
        Invisible = 0,  //cannot see the actor
        Cloaked,        //actor cloaked but is in the same team or controlled by the player (so can be seen)
        Visible         //actor is not cloaked so it is visible by anyone
    };
    static CloakState GetCloakState( const Actor& actor );
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    core::ProgramState& mProgramState;
};

} // namespace engine

#endif//INCLUDED_ENGINE_CLOAK_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "cloak_system" -t "cloak"
