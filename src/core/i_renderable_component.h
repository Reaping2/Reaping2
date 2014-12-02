#ifndef INCLUDED_CORE_I_RENDERABLE_COMPONENT_H
#define INCLUDED_CORE_I_RENDERABLE_COMPONENT_H
#include "core/component.h"
#include "core/renderable_layer.h"

class IRenderableComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IRenderableComponent)
    virtual RenderableLayer::Type const& GetLayer()const=0;
    virtual void SetLayer( RenderableLayer::Type Lay )=0;
    virtual int32_t const& GetZOrder()const=0;
    virtual void SetZOrder( int32_t ZOrder )=0;
protected:
    friend class ComponentFactory;
};

#endif//INCLUDED_CORE_I_RENDERABLE_COMPONENT_H