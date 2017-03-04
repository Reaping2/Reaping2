#include "player_camera.h"
#include "platform/settings.h"

PlayerCamera::PlayerCamera( Projection const& Proj )
    : Camera( Proj )
    , mScene( Scene::Get() )
    , mAllowedDistance( 400, 400 )
{
    mPinCenterToPlayer = Settings::Get().GetBool( "camera.pin_center_to_player", true );
}

void PlayerCamera::UpdateAllowedCenterRegion()
{
    mAllowedCenterRegion = mScene.GetDimensions();
    glm::vec4 const& VisRegion = mProjection.GetVisibleRegion();
    mAllowedCenterRegion -= VisRegion;
}

void PlayerCamera::Update()
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
    if( mPinCenterToPlayer )
    {
        mCenter = glm::vec2( px, py );
    }
    else
    {
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
    }
    mCenter = glm::round( mCenter );
    UpdateMatrices();
}


