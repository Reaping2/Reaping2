#ifndef INCLUDED_RENDER_PLAYER_CAMERA_H
#define INCLUDED_RENDER_PLAYER_CAMERA_H
#include "camera.h"
#include "core/scene.h"

class PlayerCamera : public Camera
{
    glm::vec2 mAllowedDistance;
    glm::vec4 mAllowedCenterRegion;
    bool mPinCenterToPlayer = true;
    Scene& mScene;
    void UpdateAllowedCenterRegion();
public:
    PlayerCamera( Projection const& Proj );
    void Update();
};

#endif//INCLUDED_RENDER_PLAYER_CAMERA_H
