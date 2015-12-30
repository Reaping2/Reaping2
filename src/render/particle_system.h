#ifndef INCLUDED_ENGINE_PARTICLE_SYSTEM_H
#define INCLUDED_ENGINE_PARTICLE_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace render {

class ParticleSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(ParticleSystem)
    ParticleSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

INSTANTIATE_SYSTEM( ParticleSystem );

} // namespace render

#endif//INCLUDED_ENGINE_PARTICLE_SYSTEM_H

//command:  "../../build/tools/classgenerator/classgenerator" -g "system" -c "particle_system" -t "emitter"
