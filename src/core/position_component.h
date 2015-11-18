#ifndef INCLUDED_CORE_POSITION_COMPONENT_H
#define INCLUDED_CORE_POSITION_COMPONENT_H

#include "i_position_component.h"
#include "core/property_loader.h"

class PositionComponent : public IPositionComponent
{
public:
    virtual double const& GetX()const; //TODO: component style, its not a really good idea for models, if components are changed
    virtual double const& GetY()const;
    virtual double const& GetOrientation()const;
    virtual void SetX( double X );
    virtual void SetY( double Y );
    virtual void SetOrientation( double Orientation );
    virtual void Save(Json::Value& component);
protected:
    PositionComponent();
    friend class ComponentFactory;
private:
    double mX;
    double mY;
    double mOrientation;
};

class PositionComponentLoader: public ComponentLoader<PositionComponent>
{
    virtual void BindValues();
protected:
    PositionComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_POSITION_COMPONENT_H