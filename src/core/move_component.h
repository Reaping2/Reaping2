#ifndef INCLUDED_CORE_MOVE_COMPONENT_H
#define INCLUDED_CORE_MOVE_COMPONENT_H

#include "i_move_component.h"
#include "core/property_loader.h"



class MoveComponent : public IMoveComponent
{
public:
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
    friend class ComponentFactory;
private:
    double mHeading;
    double mSpeed;
    double mSpeedX;
    double mSpeedY;
    void UpdateProjections();
};

class MoveComponentLoader: public ComponentLoader<MoveComponent>
{
    virtual void BindValues();
protected:
    MoveComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_MOVE_COMPONENT_H