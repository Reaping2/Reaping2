#ifndef INCLUDED_CORE_I_ACCELERATION_COMPONENT_H
#define INCLUDED_CORE_I_ACCELERATION_COMPONENT_H
#include "component.h"
#include "platform/export.h"

class IAccelerationComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE( IAccelerationComponent )
    virtual double const& GetAcceleration() const = 0;
    virtual double const& GetMinSpeed() const = 0;
    virtual double const& GetMaxSpeed() const = 0;
    virtual void SetAcceleration( double acceleration ) = 0;
    virtual void SetMinSpeed( double minSpeed ) = 0;
    virtual void SetMaxSpeed( double maxSpeed ) = 0;

protected:
    friend class ComponentFactory;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IAccelerationComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<Component>( *this );
}


REAPING2_CLASS_EXPORT_KEY2( IAccelerationComponent, IAccelerationComponent, "i_acceleration_component" );
#endif//INCLUDED_CORE_I_ACCELERATION_COMPONENT_H