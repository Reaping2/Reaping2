#ifndef INCLUDED_CORE_I_MOVE_COMPONENT_H
#define INCLUDED_CORE_I_MOVE_COMPONENT_H
#include "component.h"

class IMoveComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IMoveComponent)
    virtual double const& GetHeading() const=0; //TODO: component style, its not a really good idea for models, if components are changed
    virtual double const& GetSpeed() const=0;
    virtual double const& GetSpeedX() const=0;
    virtual double const& GetSpeedY() const=0;
    virtual void SetHeading( double Heading )=0;
    virtual void SetSpeed( double Speed )=0;
    virtual void SetSpeedX( double SpeedX )=0;
    virtual void SetSpeedY( double SpeedY )=0;
protected:
    friend class ComponentFactory;
};

#endif//INCLUDED_CORE_I_MOVE_COMPONENT_H