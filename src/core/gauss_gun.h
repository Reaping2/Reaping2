#pragma once
#ifndef INCLUDED_GAUSS_GUN_H
#define INCLUDED_GAUSS_GUN_H

#include "core/weapon.h"
#include "platform/export.h"

class GaussGun : public Weapon
{
    GaussGun( int32_t Id );
    friend class Factory<Item>;
    GaussGun();

    void InitMembers();

    double mChargeTime;
    double mCurrentCharge;
public:
    virtual glm::vec3 GetMouseColor() const;
    virtual void StartCharge();
    virtual void EndCharge();
    virtual bool IsShootingAlt() const;
    virtual bool IsCharging() const;
    virtual double ChargeTime() const;
public:
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


REAPING2_CLASS_EXPORT_KEY2(GaussGun, GaussGun,"gauss_gun");
#endif // INCLUDED_GAUSS_GUN_H

