#ifndef INCLUDED_ENGINE_DETONATE_ON_HIT_SYSTEM_H
#define INCLUDED_ENGINE_DETONATE_ON_HIT_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class DetonateOnHitSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(DetonateOnHitSystem)
    DetonateOnHitSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_DETONATE_ON_HIT_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "detonate_on_hit_system" -t "detonate_on_hit"
