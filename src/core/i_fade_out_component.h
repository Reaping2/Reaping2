#ifndef INCLUDED_CORE_I_FADE_OUT_COMPONENT_H
#define INCLUDED_CORE_I_FADE_OUT_COMPONENT_H
#include "component.h"

class IFadeOutComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IFadeOutComponent)
    virtual void Update( double Seconds )=0;    
    virtual double GetSecsToEnd()=0;
    virtual void SetSecsToEnd( double secsToEnd )=0;
protected:
	friend class ComponentFactory;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void IFadeOutComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_FADE_OUT_COMPONENT_H