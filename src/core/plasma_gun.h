#ifndef INCLUDED_CORE_PLASMA_GUN_H
#define INCLUDED_CORE_PLASMA_GUN_H
#include "core/weapon.h"

class PlasmaGun : public Weapon
{
    PlasmaGun( int32_t Id );
    friend class Factory<Item>;
    PlasmaGun();

    void InitMembers();

public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void PlasmaGun::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Weapon>(*this);
}

#endif//INCLUDED_CORE_PLASMA_GUN_H
