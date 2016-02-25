#include "platform/i_platform.h"
#include "core/gauss_gun.h"
#include "core/audible_event.h"
#include "core/i_move_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

GaussGun::GaussGun( int32_t Id )
    : Weapon( Id )
    , mChargeTime( 1.0 )
    , mCurrentCharge( 0.0 )
{
}

GaussGun::GaussGun()
    : Weapon( -1 )
    , mChargeTime( 1.0 )
    , mCurrentCharge( 0.0 )
{
}

glm::vec3 GaussGun::GetMouseColor() const
{
    if( GetCooldown() <= 0.0 && IsCharging() )
    {
        double state = std::max( 0.0, std::min( 1.0, ( glfwGetTime() - mCurrentCharge ) / mChargeTime ) );
        return glm::vec3( state, state, 1.0 - state );
    }
    return Weapon::GetMouseColor();
}

void GaussGun::StartCharge()
{
    if( mCurrentCharge >= 0.0 )
        return;
    mCurrentCharge = glfwGetTime();
}

void GaussGun::EndCharge()
{
    mCurrentCharge = -1.;
}

bool GaussGun::IsShootingAlt() const
{
    return Weapon::IsShootingAlt() &&
        mCurrentCharge > 0.0 &&
        glfwGetTime() - mCurrentCharge >= mChargeTime;

}

bool GaussGun::IsCharging() const
{
    return mCurrentCharge > 0.0;
}

double GaussGun::ChargeTime() const
{
    return mChargeTime;
}

void GaussGun::SetChargeTime(bool chargeTime)
{
    mChargeTime=chargeTime;
}


void GaussGunLoader::BindValues()
{
    Bind("charge_time",func_double(&GaussGun::SetChargeTime));
}

GaussGunLoader::GaussGunLoader()
{
    SetBase<WeaponLoader>();
}
BOOST_CLASS_EXPORT_IMPLEMENT(GaussGun);
