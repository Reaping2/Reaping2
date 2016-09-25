#ifndef INCLUDED_RENDER_VISUAL_BOX_MULTIPLIER_COMPONENT_H
#define INCLUDED_RENDER_VISUAL_BOX_MULTIPLIER_COMPONENT_H

#include "render/i_visual_box_multiplier_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class ComponentLoaderFactory;

namespace render {

class VisualBoxMultiplierComponent : public IVisualBoxMultiplierComponent
{
public:
    VisualBoxMultiplierComponent();
    virtual void SetWidth(double width);
    virtual double GetWidth()const;
    virtual void SetHeight(double height);
    virtual double GetHeight()const;
protected:
    friend class ComponentFactory;
    double mWidth;
    double mHeight;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void VisualBoxMultiplierComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<IVisualBoxMultiplierComponent>(*this);
    ar& mWidth;
    ar& mHeight;
}

class VisualBoxMultiplierComponentLoader : public ComponentLoader<VisualBoxMultiplierComponent>
{
    virtual void BindValues();
protected:
    VisualBoxMultiplierComponentLoader();
    friend class ::ComponentLoaderFactory;
};

} // namespace render

REAPING2_CLASS_EXPORT_KEY2( render__VisualBoxMultiplierComponent, render::VisualBoxMultiplierComponent, "visual_box_multiplier_component" );

#endif//INCLUDED_RENDER_VISUAL_BOX_MULTIPLIER_COMPONENT_H

//command:  "../../build-reap/bin/classgenerator" -g "component" -c "visual_box_multiplier_component" -m "double-width double-height"
