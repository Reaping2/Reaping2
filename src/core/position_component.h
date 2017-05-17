#ifndef INCLUDED_CORE_POSITION_COMPONENT_H
#define INCLUDED_CORE_POSITION_COMPONENT_H

#include "i_position_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class PositionComponent : public IPositionComponent
{
public:
    virtual double const& GetX()const; //TODO: component style, its not a really good idea for models, if components are changed
    virtual double const& GetY()const;
    virtual double const& GetOrientation()const;
    virtual void SetX( double X );
    virtual void SetY( double Y );
    virtual void AddX( double X );
    virtual void AddY( double Y );
    virtual void SetOrientation( double Orientation );
    virtual void Save( Json::Value& component );
protected:
    PositionComponent();
    friend class ComponentFactory;
private:
    double mX;
    double mY;
    double mOrientation;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void PositionComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<IPositionComponent>( *this );
    ar& mX;
    ar& mY;
    ar& mOrientation;
}

class PositionComponentLoader: public ComponentLoader<PositionComponent>
{
public:
    DEFINE_COMPONENT_LOADER_BASE( PositionComponentLoader )

    static std::unique_ptr<PositionComponentLoader> FromActor( Actor const& actor );
    static std::unique_ptr<PositionComponentLoader> FromPosition( double x, double y );
private:
    virtual void BindValues();
protected:
    PositionComponentLoader();
    friend class ComponentLoaderFactory;
};


REAPING2_CLASS_EXPORT_KEY2( PositionComponent, PositionComponent, "position_component" );
#endif//INCLUDED_CORE_POSITION_COMPONENT_H
