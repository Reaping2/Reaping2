#ifndef INCLUDED_CORE_I_MOVE_COMPONENT_H
#define INCLUDED_CORE_I_MOVE_COMPONENT_H
#include "component.h"
#include "platform/event.h"
#include "buffable.h"
#include <boost/serialization/export.hpp>

struct MoveChangedEvent : public platform::Event
{
    Opt<Actor> mActor;
    MoveChangedEvent( Opt<Actor> actor ): mActor( actor ){}
};

class IMoveComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IMoveComponent)
    virtual double const& GetHeading() const=0; 
    virtual double const& GetHeadingModifier() const=0; 
    virtual double GetSpeedX() const=0;
    virtual double GetSpeedY() const=0;
    virtual bool IsMoving() const=0;
    virtual void SetHeading( double Heading )=0;
    virtual void SetHeadingModifier( double HeadingModifier )=0;
    virtual void SetSpeed( double Speed )=0;
    virtual bool GetMoving() const=0;
    virtual void SetMoving( bool moving )=0;
    virtual Buffable<double>& GetSpeed()=0;
    virtual void SetRooted(bool rooted)=0;
    virtual bool IsRooted()const=0;
protected:
    friend class ComponentFactory;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void IMoveComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}


BOOST_CLASS_EXPORT_KEY2(IMoveComponent,"i_move_component");
#endif//INCLUDED_CORE_I_MOVE_COMPONENT_H