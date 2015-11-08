#include "platform/i_platform.h"
#include "core/weapon.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/scene.h"

Weapon::Weapon( int32_t Id )
    : Item( Id )
    , mActorFactory(ActorFactory::Get())
    , mCooldown( 0.0 )
    , mShootCooldown( 1.0 )
    , mShootAltCooldown( 1.0 )
    , mScatter( 0 )
    , mAltScatter( 0 )
    , mShoot(false)
    , mShootAlt(false)
{
    mType = ItemType::Weapon;
}

bool Weapon::IsShoot() const
{
    return mShoot;
}

void Weapon::SetShoot(bool shoot)
{
    mShoot = shoot;
}

bool Weapon::IsShootAlt() const
{
    return mShootAlt;
}

void Weapon::SetShootAlt(bool shoot)
{
    mShootAlt = shoot;
}

double Weapon::GetCooldown() const
{
    return mCooldown;
}

void Weapon::SetCooldown(double cooldown)
{
    mCooldown = cooldown;
}

double Weapon::GetShootCooldown() const
{
    return mShootCooldown;
}

void Weapon::SetShootCooldown(double cooldown)
{
    mShootCooldown = cooldown;
}

double Weapon::GetShootAltCooldown() const
{
    return mShootAltCooldown;
}

void Weapon::SetShootAltCooldown(double cooldown)
{
    mShootAltCooldown = cooldown;
}

uint32_t Weapon::GetScatter() const
{
    return mScatter;
}

void Weapon::SetScatter(uint32_t scatter)
{
    mScatter = scatter;
}

uint32_t Weapon::GetAltScatter() const
{
    return mAltScatter;
}

void Weapon::SetAltScatter(uint32_t scatter)
{
    mAltScatter = scatter;
}