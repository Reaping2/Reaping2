#ifndef INCLUDED_CORE_ION_GUN_H
#define INCLUDED_CORE_ION_GUN_H
#include "core/weapon.h"
#include <boost/serialization/export.hpp>

class IonGun : public Weapon
{
    IonGun( int32_t Id );
    friend class Factory<Item>;
    IonGun();

    void InitMembers();

public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void IonGun::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Weapon>(*this);
}


BOOST_CLASS_EXPORT_KEY2(IonGun,"ion_gun");
#endif//INCLUDED_CORE_ION_GUN_H
