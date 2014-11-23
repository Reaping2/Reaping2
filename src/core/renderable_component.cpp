#include "renderable_component.h"


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
