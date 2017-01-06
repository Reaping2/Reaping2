#include "health_component.h"
#include "i_position_component.h"
#include "platform/auto_id.h"
#include "platform/event.h"
#include "core/damage_taken_event.h"
#include "renderable_component.h"
#include "platform/i_platform.h"
#include "core/scene.h"
#include "core/opt.h"
#include "core/renderable_layer.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

using platform::AutoId;
using platform::EventServer;

HealthComponent::HealthComponent()
    : mHP( 1 )
    , mDamage( 0 )
    , mHeal( 0 )
    , mAlive( true )
    , mTimeOfDeath( 0 )
    , mLastDamageOwnerGUID( -1 )
    , mLastDamageTime( -1.0 )
{
    mMaxHP.mBase.Init( 100.0, 0.0, 10000.0 );
    mMaxHP.mFlat.Init( 0.0, 0.0, 1000.0 );
    mMaxHP.mPercent.Init( 0.0, 0.0, 100.0 );
}

void HealthComponent::Update( double Seconds )
{
}

int32_t const& HealthComponent::GetHP() const
{
    return mHP;
}

void HealthComponent::SetHP( int32_t Hp )
{
    mHP = Hp;
}

bool HealthComponent::IsAlive() const
{
    return mAlive;
}

void HealthComponent::SetAlive( bool alive )
{
    mAlive = alive;
}

double HealthComponent::GetTimeOfDeath() const
{
    return mTimeOfDeath;
}

void HealthComponent::SetTimeOfDeath( double timeOfDeath )
{
    mTimeOfDeath = timeOfDeath;
}

void HealthComponent::TakeDamage( int32_t damage )
{
    mDamage += damage;
}

void HealthComponent::ResetDamage()
{
    mDamage = 0;
}

int32_t HealthComponent::GetDamage()
{
    return mDamage;
}

void HealthComponent::TakeHeal( int32_t heal )
{
    mHeal += heal;
}

int32_t HealthComponent::GetHeal()
{
    return mHeal;
}

void HealthComponent::ResetHeal()
{
    mHeal = 0;
}

Buffable<int32_t>& HealthComponent::GetMaxHP()
{
    return mMaxHP;
}

void HealthComponent::SetHPandMaxHP( int32_t Hp )
{
    mMaxHP.mBase.Set( Hp );
    SetHP( mMaxHP.Get() );
}

void HealthComponent::SetLastDamageOwnerGUID( int32_t lastDamageOwnerGUID )
{
    mLastDamageOwnerGUID = lastDamageOwnerGUID;
}

int32_t HealthComponent::GetLastDamageOwnerGUID()const
{
    return mLastDamageOwnerGUID;
}

void HealthComponent::SetLastDamageTime( double lastDamageTime )
{
    mLastDamageTime = lastDamageTime;
}

double HealthComponent::GetLastDamageTime() const
{
    return mLastDamageTime;
}


void HealthComponentLoader::BindValues()
{
    Bind( "hp", func_int32_t( &HealthComponent::SetHPandMaxHP ) );
}

HealthComponentLoader::HealthComponentLoader()
{
}

REAPING2_CLASS_EXPORT_IMPLEMENT( HealthComponent, HealthComponent );
