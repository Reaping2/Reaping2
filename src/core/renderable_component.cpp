#include "core/renderable_component.h"
#include "core/actor.h"
#include "platform/i_platform.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

RenderableComponent::RenderableComponent()
    : mLayer( RenderableLayer::Background )
    , mZOrder( 0 )
{
}

RenderableLayer::Type const& RenderableComponent::GetLayer() const
{
    return mLayer;
}

void RenderableComponent::SetLayer( RenderableLayer::Type Lay )
{
    mLayer = Lay;
}

int32_t const& RenderableComponent::GetZOrder() const
{
    return mZOrder;
}

void RenderableComponent::SetZOrder( int32_t ZOrder )
{
    mZOrder = ZOrder;
}

RenderableComponentModifier::RenderableComponentModifier( RenderableLayer::Type Lay, int32_t ZOrder )
    : mLayer( Lay )
    , mZOrder( ZOrder )
{
}

void RenderableComponentModifier::operator()( Opt<Actor>& Obj )
{
    Obj->Get<IRenderableComponent>()->SetLayer( mLayer );
    Obj->Get<IRenderableComponent>()->SetZOrder( mZOrder );
}

void RenderableComponentLoader::BindValues()
{
    Bind( "zorder", func_int32_t( &RenderableComponent::SetZOrder ) );
    std::string istr;
    if( Json::GetStr( ( *mSetters )["layer"], istr ) )
    {
        Bind<RenderableLayer::Type>( &RenderableComponent::SetLayer, mRenderableLayer( AutoId( istr ) ) );
    }
}

RenderableComponentLoader::RenderableComponentLoader()
    : mRenderableLayer( RenderableLayer::Get() )
{
}

REAPING2_CLASS_EXPORT_IMPLEMENT( RenderableComponent, RenderableComponent );
