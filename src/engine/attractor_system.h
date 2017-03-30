#ifndef INCLUDED_ENGINE_ATTRACTOR_SYSTEM_H
#define INCLUDED_ENGINE_ATTRACTOR_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class AttractorSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(AttractorSystem)
    AttractorSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_ATTRACTOR_SYSTEM_H

//command:  "classgenerator.exe" -g "component" -c "attractor_component" -m "AttractorDescs_t-attractorDescs"
