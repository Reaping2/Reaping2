#ifndef INCLUDED_RENDER_CAMERA_H
#define INCLUDED_RENDER_CAMERA_H
#include "platform/i_platform.h"
#include "projection.h"

class Camera
{
protected:
    glm::mat4 mView;
    glm::mat4 mInverseView;
    glm::vec2 mCenter;
    Projection const& mProjection;
    void UpdateMatrices();
public:
    Camera( Projection const& Proj );
    glm::vec2 GetCenter() const;
    glm::vec4 VisibleRegion() const;
    void SetCenter( glm::vec2 const& center );
    glm::mat4 const& GetView()const;
    glm::mat4 const& GetInverseView()const;
};

#endif//INCLUDED_RENDER_CAMERA_H
