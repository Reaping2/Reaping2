#ifndef INCLUDED_RENDER_CAMERA_H
#define INCLUDED_RENDER_CAMERA_H

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
    void Update();
    glm::mat4 const& GetView()const;
    glm::mat4 const& GetInverseView()const;
};

#endif//INCLUDED_RENDER_CAMERA_H
