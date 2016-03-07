#ifndef INCLUDED_ENGINE_SCORE_ON_DEATH_SYSTEM_H
#define INCLUDED_ENGINE_SCORE_ON_DEATH_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class ScoreOnDeathSystem : public System
{
public:
    DEFINE_SYSTEM_BASE( ScoreOnDeathSystem )
    ScoreOnDeathSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_SCORE_ON_DEATH_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "score_on_death_system" -t "score_on_death"
