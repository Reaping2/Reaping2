#ifndef INCLUDED_CORE_MOVE_COMPONENT_H
#define INCLUDED_CORE_MOVE_COMPONENT_H

#include "i_move_component.h"

class MoveComponent : public IMoveComponent
{
public:
    DEFINE_COMPONENT_IMPLEMENTATION(MoveComponent)
    virtual double const& GetHeading() const; //TODO: component style, its not a really good idea for models, if components are changed
    virtual double const& GetSpeed() const;
    virtual double const& GetSpeedX() const;
    virtual double const& GetSpeedY() const;
    virtual void SetHeading( double Heading );
    virtual void SetSpeed( double Speed );
    virtual void SetSpeedX( double SpeedX );
    virtual void SetSpeedY( double SpeedY );
protected:
    MoveComponent();
    friend class ComponentRepo;
private:
    double mHeading;
    double mSpeed;
    double mSpeedX;
    double mSpeedY;
};

#endif//INCLUDED_CORE_MOVE_COMPONENT_H