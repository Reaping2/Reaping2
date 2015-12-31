#ifndef INCLUDED_CORE_MOVE_COMPONENT_H
#define INCLUDED_CORE_MOVE_COMPONENT_H

#include "i_move_component.h"
#include "core/property_loader.h"

class MoveComponent : public IMoveComponent
{
public:
    virtual double const& GetHeading() const; 
    virtual double const& GetHeadingModifier() const; 
    virtual double GetSpeedX() const;
    virtual double GetSpeedY() const;
    virtual bool GetMoving() const;
    virtual bool IsMoving() const;
    virtual void SetSpeed( double Speed ); //dont use this
    virtual void SetHeading( double Heading );
    virtual void SetHeadingModifier( double HeadingModifier );
    virtual void SetMoving( bool moving );
    virtual Buffable<double>& GetSpeed();
    virtual void SetRooted(bool rooted);
    virtual bool IsRooted()const;
protected:
    MoveComponent();
    friend class ComponentFactory;
private:
    double mHeading;
    double mHeadingCos;
    double mHeadingSin;
    double mHeadingModifier;
    double mSpeedX;
    double mSpeedY;
    bool mMoving;
    Buffable<double> mSpeed;
    bool mRooted;
};

class MoveComponentLoader: public ComponentLoader<MoveComponent>
{
    virtual void BindValues();
protected:
    MoveComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_MOVE_COMPONENT_H