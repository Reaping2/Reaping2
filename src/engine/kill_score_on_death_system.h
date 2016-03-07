#ifndef INCLUDED_ENGINE_KILL_SCORE_ON_DEATH_SYSTEM_H
#define INCLUDED_ENGINE_KILL_SCORE_ON_DEATH_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class KillScoreOnDeathSystem : public System
{
public:
    DEFINE_SYSTEM_BASE( KillScoreOnDeathSystem )
    KillScoreOnDeathSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_KILL_SCORE_ON_DEATH_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "kill_score_on_death_system" -t "kill_score_on_death"
