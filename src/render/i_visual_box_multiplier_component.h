#ifndef INCLUDED_RENDER_I_VISUAL_BOX_MULTIPLIER_COMPONENT_H
#define INCLUDED_RENDER_I_VISUAL_BOX_MULTIPLIER_COMPONENT_H

#include "core/component.h"

namespace render {

class IVisualBoxMultiplierComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(render::IVisualBoxMultiplierComponent)
    virtual void SetWidth(double width)=0;
    virtual double GetWidth()const=0;
    virtual void SetHeight(double height)=0;
    virtual double GetHeight()const=0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IVisualBoxMultiplierComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Component>(*this);
}

} // namespace render

#endif//INCLUDED_RENDER_I_VISUAL_BOX_MULTIPLIER_COMPONENT_H

//command:  "../../build-reap/bin/classgenerator" -g "component" -c "visual_box_multiplier_component" -m "double-width double-height"
