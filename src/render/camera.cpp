#include "i_render.h"
#include "camera.h"

Camera::Camera( Projection const& Proj )
    : mScene( Scene::Get() )
    , mAllowedDistance( 0.5, 0.5 )
    , mProjection( Proj )
{
}

void Camera::UpdateMatrices()
{
    mView = glm::translate( glm::vec3( -mCenter.x, -mCenter.y, 0.f ) );
    mInverseView = glm::inverse( mView );
}

void Camera::UpdateAllowedCenterRegion()
{
    mAllowedCenterRegion = mScene.GetDimensions();
    glm::vec4 const& VisRegion = mProjection.GetVisibleRegion();
    mAllowedCenterRegion -= VisRegion;
}

void Camera::Update()
{
    ModelValue const& PlayerModel = RootModel::Get()["player"];
    if( !PlayerModel.IsValid() )
    {
        return;
    }
    double px = PlayerModel["x"];
    double py = PlayerModel["y"];
    if( mCenter.x < px - mAllowedDistance.x )
    {
        mCenter.x = ( float )px - mAllowedDistance.x;
    }
    else if( mCenter.x > px + mAllowedDistance.x )
    {
        mCenter.x = ( float )px + mAllowedDistance.x;
    }
    if( mCenter.y < py - mAllowedDistance.y )
    {
        mCenter.y = ( float )py - mAllowedDistance.y;
    }
    else if( mCenter.y > py + mAllowedDistance.y )
    {
        mCenter.y = ( float )py + mAllowedDistance.y;
    }
    UpdateAllowedCenterRegion();
    if( mCenter.x < mAllowedCenterRegion.x )
    {
        mCenter.x = mAllowedCenterRegion.x;
    }
    if( mCenter.x > mAllowedCenterRegion.z )
    {
        mCenter.x = mAllowedCenterRegion.z;
    }
    if( mCenter.y < mAllowedCenterRegion.y )
    {
        mCenter.y = mAllowedCenterRegion.y;
    }
    if( mCenter.y > mAllowedCenterRegion.w )
    {
        mCenter.y = mAllowedCenterRegion.w;
    }
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

