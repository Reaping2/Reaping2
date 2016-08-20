#ifndef INCLUDED_RENDER_CAMERA_H
#define INCLUDED_RENDER_CAMERA_H
#include "platform/i_platform.h"
#include "core/scene.h"
#include "projection.h"

class Camera
{
    glm::mat4 mView;
    glm::mat4 mInverseView;
    glm::vec2 mCenter;
    glm::vec2 mAllowedDistance;
    glm::vec4 mAllowedCenterRegion;
    Scene& mScene;
    Projection const& mProjection;
    void UpdateMatrices();
    void UpdateAllowedCenterRegion();
public:
    Camera( Projection const& Proj );
    glm::vec2 GetCenter() const;
    glm::vec4 VisibleRegion() const;
    void Update();
    glm::mat4 const& GetView()const;
    glm::mat4 const& GetInverseView()const;
};

#endif//INCLUDED_RENDER_CAMERA_H
