#ifndef INCLUDED_CORE_ROCKET_LAUNCHER_H
#define INCLUDED_CORE_ROCKET_LAUNCHER_H
#include "core/weapon.h"
#include <boost/serialization/export.hpp>

class RocketLauncher :	public Weapon
{
public:
	RocketLauncher(int32_t Id);
    RocketLauncher();
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void RocketLauncher::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Weapon>(*this);
}


class RocketLauncherLoader: public ItemLoader<RocketLauncher>
{
public:
    virtual void BindValues();
    RocketLauncherLoader();
    friend class ItemLoaderFactory;
};

BOOST_CLASS_EXPORT_KEY2(RocketLauncher,"rocket_launcher");
#endif // INCLUDED_CORE_ROCKET_LAUNCHER_H
