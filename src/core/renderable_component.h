#ifndef INCLUDED_CORE_RENDERABLE_COMPONENT_H
#define INCLUDED_CORE_RENDERABLE_COMPONENT_H

#include "i_renderable_component.h"

class Actor;

class RenderableComponentModifier
{
public:
    RenderableComponentModifier(IRenderableComponent::Layer Lay,int32_t ZOrder);
    void operator()(Opt<Actor>& Obj);

protected:
    IRenderableComponent::Layer mLayer;
    int32_t mZOrder;
};

class RenderableCompononent : public IRenderableComponent
{
public:
    virtual IRenderableComponent::Layer const& GetLayer()const;
    virtual void SetLayer( IRenderableComponent::Layer Lay );
    virtual int32_t const& GetZOrder()const;
    virtual void SetZOrder( int32_t ZOrder );
protected:
    RenderableCompononent();
    friend class ComponentFactory;
    IRenderableComponent::Layer mLayer;
    int32_t mZOrder;
};

#endif//INCLUDED_CORE_RENDERABLE_COMPONENT_H