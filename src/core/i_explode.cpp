#include "i_explode.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

IExplode::IExplode()
    : mExplosionProjectile(-1)
    , mCount(1)
    , mExplosionScatter(0.0)
    , mDistribution(ExplodeDistributionType::Normal)
    , mSecsToEndVariance(0.0)
    , mPositionVariance(0.0)
    , mSpeedVariance(0.0)
{
}

void IExplode::SetExplosionProjectile(int32_t explosionProjectile)
{
    mExplosionProjectile=explosionProjectile;
}

int32_t IExplode::GetExplosionProjectile()const
{
    return mExplosionProjectile;
}

void IExplode::SetCount(int32_t count)
{
    mCount=count;
}

int32_t IExplode::GetCount()const
{
    return mCount;
}

void IExplode::SetExplosionScatter(double scatter)
{
    mExplosionScatter=scatter;
}

double IExplode::GetExplosionScatter()const
{
    return mExplosionScatter;
}

void IExplode::SetDistribution(ExplodeDistributionType::Type distribution)
{
    mDistribution=distribution;
}

ExplodeDistributionType::Type IExplode::GetDistribution()const
{
    return mDistribution;
}

void IExplode::SetSecsToEndVariance(double secsToEndVariance)
{
    mSecsToEndVariance=secsToEndVariance;
}

double IExplode::GetSecsToEndVariance()const
{
    return mSecsToEndVariance;
}

void IExplode::SetPositionVariance(int32_t positionVariance)
{
    mPositionVariance=positionVariance;
}

int32_t IExplode::GetPositionVariance()const
{
    return mPositionVariance;
}

void IExplode::SetSpeedVariance(double speedVariance)
{
    mSpeedVariance=speedVariance;
}

double IExplode::GetSpeedVariance()const
{
    return mSpeedVariance;
}

IExplode::~IExplode()
{

}

void IExplode::SetAddActorRadius(bool addRadius)
{
    mAddActorRadius=addRadius;
}

bool IExplode::IsAddActorRadius() const
{
    return mAddActorRadius;
}

REAPING2_CLASS_EXPORT_IMPLEMENT(IExplode, IExplode);
