#ifndef INCLUDED_CORE_FADE_OUT_COMPONENT_H
#define INCLUDED_CORE_FADE_OUT_COMPONENT_H

#include "core/i_fade_out_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class FadeOutComponent : public IFadeOutComponent
{
public:
    virtual void Update( double Seconds );
    virtual double GetSecsToEnd();
    virtual void SetSecsToEnd( double secsToEnd );
    FadeOutComponent();
    friend class ComponentFactory;

    double mSecsToEnd;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void FadeOutComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<IFadeOutComponent>(*this);
    ar & mSecsToEnd;
}

class FadeOutComponentLoader: public ComponentLoader<FadeOutComponent>
{
    virtual void BindValues();
public:
    FadeOutComponentLoader();
    friend class ComponentLoaderFactory;
};


REAPING2_CLASS_EXPORT_KEY2(FadeOutComponent, FadeOutComponent,"fade_out_component");
#endif//INCLUDED_CORE_FADE_OUT_COMPONENT_H