#include "render/visual_box_multiplier_component.h"

namespace render {

VisualBoxMultiplierComponent::VisualBoxMultiplierComponent()
    : mWidth(1.0f)
    , mHeight(1.0f)
{
}

void VisualBoxMultiplierComponent::SetWidth(double width)
{
    mWidth=width;
}

double VisualBoxMultiplierComponent::GetWidth()const
{
    return mWidth;
}

void VisualBoxMultiplierComponent::SetHeight(double height)
{
    mHeight=height;
}

double VisualBoxMultiplierComponent::GetHeight()const
{
    return mHeight;
}



void VisualBoxMultiplierComponentLoader::BindValues()
{
}

VisualBoxMultiplierComponentLoader::VisualBoxMultiplierComponentLoader()
{
    Bind( "width", func_double( &VisualBoxMultiplierComponent::SetWidth ) );
    Bind( "height", func_double( &VisualBoxMultiplierComponent::SetHeight ) );
}

} // namespace render

REAPING2_CLASS_EXPORT_IMPLEMENT( render__VisualBoxMultiplierComponent, render::VisualBoxMultiplierComponent );
