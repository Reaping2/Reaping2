#ifndef INCLUDED_ENGINE_EXPLODE_ON_HIT_SYSTEM_H
#define INCLUDED_ENGINE_EXPLODE_ON_HIT_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class ExplodeOnHitSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(ExplodeOnHitSystem)
    ExplodeOnHitSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_EXPLODE_ON_HIT_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "explode_on_hit_system" -t "explode_on_hit"
