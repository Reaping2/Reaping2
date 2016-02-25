#ifndef INCLUDED_CORE_SHOTGUN_H
#define INCLUDED_CORE_SHOTGUN_H
#include "core/weapon.h"
#include "i_explode.h"
#include "explode_distribution_type.h"
#include <boost/serialization/export.hpp>

class Shotgun : public Weapon, public IExplode
{
public:
    Shotgun( int32_t Id );
    Shotgun();
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

class ShotgunLoader: public ItemLoader<Shotgun>
{
public:
    virtual void BindValues();
    ShotgunLoader();
    friend class ItemLoaderFactory;
protected:
    ExplodeDistributionType& mExplodeDistributionType;
};

BOOST_CLASS_EXPORT_KEY2(Shotgun,"shotgun");
#endif//INCLUDED_CORE_SHOTGUN_H
