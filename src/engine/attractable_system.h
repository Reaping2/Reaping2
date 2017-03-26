#ifndef INCLUDED_ENGINE_ATTRACTABLE_SYSTEM_H
#define INCLUDED_ENGINE_ATTRACTABLE_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class AttractableSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(AttractableSystem)
    AttractableSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_ATTRACTABLE_SYSTEM_H


//command:  "classgenerator.exe" -g "component" -c "attractable_component" -m "int32_t-type int32_t-deceleration IAct-turnToTargetAct"
