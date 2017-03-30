#ifndef INCLUDED_ENGINE_LIGHT_SYSTEM_H
#define INCLUDED_ENGINE_LIGHT_SYSTEM_H

#include "core/scene.h"
#include "core/map_load_event.h"
#include "engine/system.h"
#include "core/actor_event.h"

namespace render {

class LightSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(LightSystem)
    LightSystem();
    std::vector<Actor*> const& GetActiveLights() const;
    double GetMaxShadow() const;
    glm::vec2 GetShadowVector() const;
    glm::vec4 GetAmbientLight() const;
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    AutoReg mOnActorEvent;
    void OnActorEvent( ActorEvent const& Evt );
    void OnMapLoad( core::MapLoadEvent const& Evt );
    Scene& mScene;
    std::vector<Actor*> mActiveLights;
    std::vector<Actor*> mActiveDummyObjects;
    double mMaxShadow = 0.6;
    AutoReg mOnMapLoad;
    glm::vec2 mShadowVec = glm::vec2( 25, -25 );
    glm::vec4 mAmbientLight = glm::vec4( 1., 1., 1., 0.6 );
};

INSTANTIATE_SYSTEM( LightSystem );

} // namespace render

#endif//INCLUDED_ENGINE_LIGHT_SYSTEM_H

//command:  "../../build-reap-release/bin/relwithdebinfo/classgenerator" -g "system" -c "light_system" -t "light"
