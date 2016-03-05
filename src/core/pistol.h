#ifndef INCLUDED_CORE_PISTOL_H
#define INCLUDED_CORE_PISTOL_H
#include "core/weapon.h"
#include "platform/export.h"

class Pistol : public Weapon
{
public:
    Pistol( int32_t Id );
    Pistol();
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void Pistol::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Weapon>(*this);
}

class PistolLoader: public ItemLoader<Pistol>
{
public:
    virtual void BindValues();
    PistolLoader();
    friend class ItemLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2(Pistol, Pistol,"pistol");
#endif//INCLUDED_CORE_PISTOL_H
