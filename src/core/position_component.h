#ifndef INCLUDED_CORE_POSITION_COMPONENT_H
#define INCLUDED_CORE_POSITION_COMPONENT_H

#include "i_position_component.h"

class PositionComponent : public IPositionComponent
{
public:
    virtual double const& GetX()const; //TODO: component style, its not a really good idea for models, if components are changed
    virtual double const& GetY()const;
    virtual double const& GetOrientation()const;
    virtual void SetX( double X );
    virtual void SetY( double Y );
    virtual void SetOrientation( double Orientation );
protected:
    PositionComponent();
    friend class ComponentFactory;
private:
    double mX;
    double mY;
    double mOrientation;
};

#endif//INCLUDED_CORE_POSITION_COMPONENT_H