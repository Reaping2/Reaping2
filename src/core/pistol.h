#ifndef INCLUDED_CORE_PISTOL_H
#define INCLUDED_CORE_PISTOL_H
#include "core/weapon.h"
#include "platform/export.h"

class Pistol : public Weapon
{
    Pistol( int32_t Id );
    friend class Factory<Item>;
    Pistol();

    void InitMembers();

public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void Pistol::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Weapon>(*this);
}


REAPING2_CLASS_EXPORT_KEY2(Pistol, Pistol,"pistol");
#endif//INCLUDED_CORE_PISTOL_H
