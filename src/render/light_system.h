#ifndef INCLUDED_ENGINE_LIGHT_SYSTEM_H
#define INCLUDED_ENGINE_LIGHT_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace render {

class LightSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(LightSystem)
    LightSystem();
    std::vector<Actor*> const& GetActiveLights() const;
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    std::vector<Actor*> mActiveLights;
    std::vector<Actor*> mActiveDummyObjects;
};

INSTANTIATE_SYSTEM( LightSystem );

} // namespace render

#endif//INCLUDED_ENGINE_LIGHT_SYSTEM_H

//command:  "../../build-reap-release/bin/relwithdebinfo/classgenerator" -g "system" -c "light_system" -t "light"
