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
    ModelValue const& mx = PlayerModel["x"];
    ModelValue const& my = PlayerModel["y"];
    if ( !mx.IsValid() || !my.IsValid() )
    {
        return;
    }
    double px = double( mx );
    double py = double( my );
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
// TODO: there might be a switch for this functionality later
//     UpdateAllowedCenterRegion();
//     if( mCenter.x < mAllowedCenterRegion.x )
//     {
//         mCenter.x = mAllowedCenterRegion.x;
//     }
//     if( mCenter.x > mAllowedCenterRegion.z )
//     {
//         mCenter.x = mAllowedCenterRegion.z;
//     }
//     if( mCenter.y < mAllowedCenterRegion.y )
//     {
//         mCenter.y = mAllowedCenterRegion.y;
//     }
//     if( mCenter.y > mAllowedCenterRegion.w )
//     {
//         mCenter.y = mAllowedCenterRegion.w;
//     }
    mCenter.x = glm::round( mCenter.x );
    mCenter.y = glm::round( mCenter.y );
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

