#ifndef INCLUDED_CORE_I_POSITION_COMPONENT_H
#define INCLUDED_CORE_I_POSITION_COMPONENT_H
#include "component.h"
#include "platform/event.h"
#include "platform/export.h"

struct PositionChangedEvent : public platform::Event
{
    Opt<Actor> mActor;
    PositionChangedEvent( Opt<Actor> actor ): mActor( actor ){}
};

class IPositionComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IPositionComponent)
	virtual double const& GetX() const =0; //TODO: component style, its not a really good idea for models, if components are changed
	virtual double const& GetY() const =0;
	virtual double const& GetOrientation() const=0;
	virtual void SetX( double X )=0;
	virtual void SetY( double Y )=0;
	virtual void SetOrientation( double Orientation )=0;
protected:
	friend class ComponentFactory;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void IPositionComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}


REAPING2_CLASS_EXPORT_KEY2(IPositionComponent, IPositionComponent,"i_position_component");
#endif//INCLUDED_CORE_I_POSITION_COMPONENT_H