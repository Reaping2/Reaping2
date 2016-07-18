#ifndef INCLUDED_CORE_RANDOM_CONTROLLER_H
#define INCLUDED_CORE_RANDOM_CONTROLLER_H

#include "core/controller_component.h"
#include "core/actor.h"
#include "core/property_loader.h"
#include "platform/export.h"

class RandomControllerComponent : public ControllerComponent
{
public:
    double GetCounter() const;
    void SetCounter( double counter );
    double GetHeadingModifier() const;
    void SetHeadingModifier( double headingModifier );

    RandomControllerComponent();

private:
    double mCounter;
    double mHeadingModifier;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void RandomControllerComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<ControllerComponent>( *this );
    ar& mCounter;
    ar& mHeadingModifier;
}

class RandomControllerComponentLoader: public ComponentLoader<RandomControllerComponent>
{
public:
    DEFINE_COMPONENT_LOADER_BASE( RandomControllerComponentLoader )
private:
    virtual void BindValues();
protected:
    RandomControllerComponentLoader();
    friend class ComponentLoaderFactory;
};


REAPING2_CLASS_EXPORT_KEY2( RandomControllerComponent, RandomControllerComponent, "random_controller_component" );
#endif//INCLUDED_CORE_RANDOM_CONTROLLER_H
