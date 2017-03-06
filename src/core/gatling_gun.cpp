#include "platform/i_platform.h"
#include "core/gatling_gun.h"
#include "core/audible_event.h"
#include "core/i_move_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

GatlingGun::GatlingGun( int32_t Id )
    : Weapon( Id )
    , mWindup( 0.0 )
    , mWindupMax( 1.0 )
    , mDeploy( 0.0 )
    , mDeployMax( 1.0 )
    , mDeployState( Undeployed )

{
}

GatlingGun::GatlingGun()
    : Weapon( -1 )
    , mWindup( 0.0 )
    , mWindupMax( 1.0 )
    , mDeploy( 0.0 )
    , mDeployMax( 1.0 )
    , mDeployState( Undeployed )
{
}

bool GatlingGun::CanSwitch() const
{
    return mDeployState == Undeployed;
}

bool GatlingGun::IsShooting() const
{
    return mWindup == mWindupMax && Weapon::IsShooting() && mDeployState == Undeployed;
}

bool GatlingGun::IsShootingAlt() const
{
    return mWindup == mWindupMax && Weapon::IsShooting() && mDeployState == Deployed;
}

void GatlingGun::SetWindup( double windup )
{
    mWindup = windup;
}

double GatlingGun::GetWindup()const
{
    return mWindup;
}

void GatlingGun::SetWindupMax( double windupMax )
{
    mWindupMax = windupMax;
}

double GatlingGun::GetWindupMax()const
{
    return mWindupMax;
}

void GatlingGun::SetDeploy( double deploy )
{
    mDeploy = deploy;
}

double GatlingGun::GetDeploy()const
{
    return mDeploy;
}

void GatlingGun::SetDeployMax( double deployMax )
{
    mDeployMax = deployMax;
}

double GatlingGun::GetDeployMax()const
{
    return mDeployMax;
}

void GatlingGun::SetDeployState( DeployState deployState )
{
    mDeployState = deployState;
}

GatlingGun::DeployState GatlingGun::GetDeployState()const
{
    return mDeployState;
}

glm::vec3 GatlingGun::GetMouseColor() const
{
    if ( mDeployState == Deploying || mDeployState == Undeploying )
    {
        return glm::vec3( 0.5 );
    }
    else if ( mReloadTime <= 0.0 && mDeployState == Deployed )
    {
        return glm::vec3( 1 / 255.0, 120 / 255.0, 196 / 255.0 );
    }
    return Weapon::GetMouseColor();
}

double GatlingGun::GetMouseSize() const
{
    if ( mDeployState == Deploying )
    {
        return ( mDeployMax - mDeploy ) / mDeployMax * 150.0;
    }
    else if ( mDeployState == Undeploying )
    {
        return ( mDeploy ) / mDeployMax * 150.0;
    }
    return Weapon::GetMouseSize();
}

std::string GatlingGun::GetMouseText() const
{
    if ( mDeployState == Deploying )
    {
        return "Deploy";
        //             "Deploy:"+boost::lexical_cast<std::string>(
        //             std::max(   //no negative deploy shown.
        //             0.0,std::floor(mDeployMax-mDeploy)
        //             ))+"%";
    }
    else if ( mDeployState == Undeploying )
    {
        return "Undeploy";
        //             "Undeploy:"+boost::lexical_cast<std::string>(
        //             std::max(   //no negative deploy shown.
        //             0.0,std::floor(mDeploy)
        //             ))+"%";
    }
    return Weapon::GetMouseText();
}

bool GatlingGun::IsMouseResizable() const
{
    return ( mDeployState == Deployed || mDeployState == Undeployed )
           && Weapon::IsMouseResizable();
}

bool GatlingGun::CanReload() const
{
    return ( mDeployState == Deployed || mDeployState == Undeployed )
           && Weapon::CanReload();
}

void GatlingGunLoader::BindValues()
{
    Bind( "windup_time", func_double( &GatlingGun::SetWindupMax ) );
    Bind( "deploy_time", func_double( &GatlingGun::SetDeployMax ) );
}

GatlingGunLoader::GatlingGunLoader()
{
    SetBase<WeaponLoader>();
}


REAPING2_CLASS_EXPORT_IMPLEMENT( GatlingGun, GatlingGun );
