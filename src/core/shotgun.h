#ifndef INCLUDED_CORE_SHOTGUN_H
#define INCLUDED_CORE_SHOTGUN_H
#include "core/weapon.h"
#include "i_explode.h"

class Shotgun : public Weapon, public IExplode
{
    Shotgun( int32_t Id );
    friend class Factory<Item>;
    Shotgun();

    void InitMembers();

public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void Shotgun::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Weapon>(*this);
    ar & boost::serialization::base_object<IExplode>(*this);
}

#endif//INCLUDED_CORE_SHOTGUN_H
