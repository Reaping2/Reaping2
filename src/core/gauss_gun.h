#pragma once
#ifndef INCLUDED_GAUSS_GUN_H
#define INCLUDED_GAUSS_GUN_H

#include "core/weapon.h"

class GaussGun : public Weapon
{
    double mChargeTime;
    double mCurrentCharge;
public:
    GaussGun( int32_t Id );
    GaussGun();
    virtual glm::vec3 GetMouseColor() const;
    virtual void StartCharge();
    virtual void EndCharge();
    virtual bool IsShootingAlt() const;
    virtual bool IsCharging() const;
    virtual double ChargeTime() const;
    virtual void SetChargeTime(bool chargeTime);
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void GaussGun::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Weapon>(*this);
    ar & mChargeTime;
    ar & mCurrentCharge;
}

class GaussGunLoader: public ItemLoader<GaussGun>
{
public:
    virtual void BindValues();
    GaussGunLoader();
    friend class ItemLoaderFactory;
};

#endif // INCLUDED_GAUSS_GUN_H

