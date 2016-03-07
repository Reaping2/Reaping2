#ifndef INCLUDED_CORE_PLASMA_GUN_H
#define INCLUDED_CORE_PLASMA_GUN_H
#include "core/weapon.h"
#include "item_loader.h"
#include "platform/export.h"

class PlasmaGun : public Weapon
{
public:
    PlasmaGun( int32_t Id );
    PlasmaGun();
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void PlasmaGun::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Weapon>( *this );
}

class PlasmaGunLoader: public ItemLoader<PlasmaGun>
{
public:
    virtual void BindValues();
    PlasmaGunLoader();
    friend class ItemLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( PlasmaGun, PlasmaGun, "plasma_gun" );
#endif//INCLUDED_CORE_PLASMA_GUN_H
