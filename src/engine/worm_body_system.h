#ifndef INCLUDED_ENGINE_WORM_BODY_SYSTEM_H
#define INCLUDED_ENGINE_WORM_BODY_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class WormBodySystem : public System
{
public:
    DEFINE_SYSTEM_BASE(WormBodySystem)
    WormBodySystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_WORM_BODY_SYSTEM_H

//command:  "classgenerator.exe" -g "component" -c "worm_body_component" -m "int32_t-headGUID bool-body int32_t-followedGUID PrevPositions_t-prevPositions"
