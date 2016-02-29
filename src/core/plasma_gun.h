#ifndef INCLUDED_CORE_PLASMA_GUN_H
#define INCLUDED_CORE_PLASMA_GUN_H
#include "core/weapon.h"
#include "item_loader.h"

class PlasmaGun : public Weapon
{
public:
    PlasmaGun( int32_t Id );
    friend class Factory<Item>;
    PlasmaGun();

    void InitMembers();

    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void PlasmaGun::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Weapon>(*this);
}

class PlasmaGunLoader: public ItemLoader<Weapon>
{
public:
    virtual void BindValues();
    PlasmaGunLoader();
    friend class ItemLoaderFactory;
};


#endif//INCLUDED_CORE_PLASMA_GUN_H
