#ifndef INCLUDED_CORE_I_RENDERABLE_COMPONENT_H
#define INCLUDED_CORE_I_RENDERABLE_COMPONENT_H
#include "component.h"

class IRenderableComponent : public Component
{
public:
    enum Layer
    {
        Background=0,
        Corpses,
        Creeps,
        Players,
        Buildings
    };
    DEFINE_COMPONENT_BASE(IRenderableComponent)
    virtual IRenderableComponent::Layer const& GetLayer()const=0;
    virtual void SetLayer( IRenderableComponent::Layer Lay )=0;
    virtual int32_t const& GetZOrder()const=0;
    virtual void SetZOrder( int32_t ZOrder )=0;
protected:
    friend class ComponentFactory;
};

#endif//INCLUDED_CORE_I_RENDERABLE_COMPONENT_H