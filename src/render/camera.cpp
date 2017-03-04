#include "i_render.h"
#include "camera.h"

Camera::Camera( Projection const& Proj )
    : mProjection( Proj )
{
}

void Camera::UpdateMatrices()
{
    mView = glm::translate( glm::vec3( -mCenter.x, -mCenter.y, 0.f ) );
    mInverseView = glm::inverse( mView );
}

glm::vec4 Camera::VisibleRegion() const
{
    return mProjection.GetVisibleRegion() + glm::vec4( mCenter, mCenter );
}

void Camera::SetCenter( glm::vec2 const& center )
{
    mCenter = round( center );
    UpdateMatrices();
}

glm::mat4 const& Camera::GetView()const
{
    return mView;
}

glm::mat4 const& Camera::GetInverseView() const
{
    return mInverseView;
}

glm::vec2 Camera::GetCenter() const
{
    return mCenter;
}

Projection const& Camera::GetProjection() const
{
    return mProjection;
}

