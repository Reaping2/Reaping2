#ifndef INCLUDED_CORE_SOLDIER_PROPERTIES_H
#define INCLUDED_CORE_SOLDIER_PROPERTIES_H

#include "platform/singleton.h"
#include "platform/i_platform.h"
#include "actor.h"

namespace core {
struct SoldierProperties
{
    int32_t mMoveSpeed;
    int32_t mAccuracy;
    int32_t mHealth;

    int32_t mPoints;
    SoldierProperties()
        :mMoveSpeed(0),mAccuracy(0),mHealth(0),mPoints(30)
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void SoldierProperties::serialize(Archive& ar, const unsigned int version)
{
    ar & mMoveSpeed;
    ar & mAccuracy;
    ar & mHealth;
    ar & mPoints;
}

} // namespace core
#endif//INCLUDED_CORE_SOLDIER_PROPERTIES_H