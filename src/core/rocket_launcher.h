#ifndef INCLUDED_CORE_ROCKET_LAUNCHER_H
#define INCLUDED_CORE_ROCKET_LAUNCHER_H
#include "core/weapon.h"
#include <boost/serialization/export.hpp>

class RocketLauncher :	public Weapon
{
	RocketLauncher(int32_t Id);
	friend class Factory<Item>;
    RocketLauncher();

    void InitMembers();

public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void RocketLauncher::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Weapon>(*this);
}


BOOST_CLASS_EXPORT_KEY2(RocketLauncher,"rocket_launcher");
#endif // INCLUDED_CORE_ROCKET_LAUNCHER_H
