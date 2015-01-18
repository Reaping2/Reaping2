#ifndef INCLUDED_CORE_FADE_OUT_COMPONENT_H
#define INCLUDED_CORE_FADE_OUT_COMPONENT_H

#include "core/i_fade_out_component.h"
#include "core/property_loader.h"

class FadeOutComponent : public IFadeOutComponent
{
public:
    virtual void Update( double Seconds );
    virtual double GetSecsToEnd();
    virtual void SetSecsToEnd( double secsToEnd );
    FadeOutComponent();
    friend class ComponentFactory;

    double mSecsToEnd;
};

class FadeOutComponentLoader: public ComponentLoader<FadeOutComponent>
{
    virtual void BindValues();
public:
    FadeOutComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_FADE_OUT_COMPONENT_H