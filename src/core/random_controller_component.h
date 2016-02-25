#ifndef INCLUDED_CORE_RANDOM_CONTROLLER_H
#define INCLUDED_CORE_RANDOM_CONTROLLER_H

#include "core/controller_component.h"
#include "core/actor.h"
#include "core/property_loader.h"
#include <boost/serialization/export.hpp>

class RandomControllerComponent : public ControllerComponent
{
public:
    double GetCounter() const;
    void SetCounter(double counter);
    double GetHeadingModifier() const;
    void SetHeadingModifier(double headingModifier);

    RandomControllerComponent();

private:
    double mCounter;
    double mHeadingModifier;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void RandomControllerComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<ControllerComponent>(*this);
    ar & mCounter;
    ar & mHeadingModifier;
}

class RandomControllerComponentLoader: public ComponentLoader<RandomControllerComponent>
{
    virtual void BindValues();
protected:
    RandomControllerComponentLoader();
    friend class ComponentLoaderFactory;
};


BOOST_CLASS_EXPORT_KEY2(RandomControllerComponent,"random_controller_component");
#endif//INCLUDED_CORE_RANDOM_CONTROLLER_H
