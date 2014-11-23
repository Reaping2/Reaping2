#include "renderable_component.h"
#include "i_core.h"

RenderableCompononent::RenderableCompononent()
    : mLayer(IRenderableComponent::Background)
    , mZOrder( 0 ) 
{
}

IRenderableComponent::Layer const& RenderableCompononent::GetLayer() const
{
    return mLayer;
}

void RenderableCompononent::SetLayer(IRenderableComponent::Layer Lay)
{
    mLayer=Lay;
}

int32_t const& RenderableCompononent::GetZOrder() const
{
    return mZOrder;
}

void RenderableCompononent::SetZOrder(int32_t ZOrder)
{
    mZOrder=ZOrder;
}

RenderableComponentModifier::RenderableComponentModifier(IRenderableComponent::Layer Lay,int32_t ZOrder)
    : mLayer(Lay)
    , mZOrder(ZOrder)
{
}

void RenderableComponentModifier::operator()(Opt<Actor>& Obj)
{
    Obj->Get<IRenderableComponent>()->SetLayer(mLayer);
    Obj->Get<IRenderableComponent>()->SetZOrder(mZOrder);
}
