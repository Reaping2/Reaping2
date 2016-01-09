#pragma once
#ifndef INCLUDED_GAUSS_GUN_H
#define INCLUDED_GAUSS_GUN_H

#include "core/weapon.h"

class GaussGun : public Weapon
{
    GaussGun( int32_t Id );
    friend class Factory<Item>;

    double mChargeTime;
    double mCurrentCharge;
public:
    virtual glm::vec3 GetMouseColor() const;
    virtual void StartCharge();
    virtual void EndCharge();
    virtual bool IsShootingAlt() const;
    virtual bool IsCharging() const;
};

#endif // INCLUDED_GAUSS_GUN_H

