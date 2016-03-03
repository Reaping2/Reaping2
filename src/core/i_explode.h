#ifndef INCLUDED_CORE_I_EXPLODE_H
#define INCLUDED_CORE_I_EXPLODE_H
#include "explode_distribution_type.h"
#include <boost/serialization/export.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/ptr_container/serialize_ptr_map.hpp>

class IExplode
{
public:
    IExplode();
    virtual ~IExplode();
    virtual void SetExplosionProjectile(int32_t explosionProjectile);
    virtual int32_t GetExplosionProjectile()const;
    virtual void SetCount(int32_t count);
    virtual int32_t GetCount()const;
    virtual void SetExplosionScatter(double scatter);
    virtual double GetExplosionScatter()const;
    virtual void SetDistribution(ExplodeDistributionType::Type distribution);
    virtual ExplodeDistributionType::Type GetDistribution()const;
    virtual void SetSecsToEndVariance(double secsToEndVariance);
    virtual double GetSecsToEndVariance()const;
    virtual void SetPositionVariance(int32_t positionVariance);
    virtual int32_t GetPositionVariance()const;
    virtual void SetSpeedVariance(double speedVariance);
    virtual double GetSpeedVariance()const;
    virtual void SetAddActorRadius(bool addRadius);
    virtual bool IsAddActorRadius()const;
protected:
    friend class ComponentFactory;
    int32_t mExplosionProjectile;
    int32_t mCount;
    double mExplosionScatter;
    ExplodeDistributionType::Type mDistribution;
    double mSecsToEndVariance;
    int32_t mPositionVariance;
    double mSpeedVariance;
    bool mAddActorRadius;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void IExplode::serialize(Archive& ar, const unsigned int version)
{
    ar & mExplosionProjectile;
    ar & mCount;
    ar & mExplosionScatter;
    ar & mDistribution;
    ar & mSecsToEndVariance;
    ar & mPositionVariance;
    ar & mSpeedVariance;
    ar & mAddActorRadius;
}

#endif//INCLUDED_CORE_I_EXPLODE_H
