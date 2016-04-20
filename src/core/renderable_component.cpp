#include "core/renderable_component.h"
#include "core/actor.h"
#include "platform/i_platform.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
#include <limits>

RenderableComponent::RenderableComponent()
    : mLayer( RenderableLayer::Background )
    , mZOrder( 0 )
    , mCastShadow( 0 )
    , mReceiveBlood( 0 )
    , mReceiveShadow( 0 )
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

int32_t RenderableComponent::GetReceiveShadow() const
{
    return mReceiveShadow;
}

void RenderableComponent::SetReceiveShadow( int32_t ReceiveShadow )
{
    mReceiveShadow = ReceiveShadow;
}

int32_t RenderableComponent::GetReceiveBlood() const
{
    return mReceiveBlood;
}

void RenderableComponent::SetReceiveBlood( int32_t ReceiveBlood )
{
    mReceiveBlood = ReceiveBlood;
}

int32_t RenderableComponent::GetCastShadow() const
{
    return mCastShadow;
}

void RenderableComponent::SetCastShadow( int32_t CastShadow )
{
    mCastShadow = CastShadow;
}

RenderableComponentModifier::RenderableComponentModifier( RenderableLayer::Type Lay, int32_t ZOrder, int32_t CastShadow, int32_t ReceiveBlood, int32_t ReceiveShadow )
    : mLayer( Lay )
    , mZOrder( ZOrder )
    , mCastShadow( CastShadow )
    , mReceiveBlood( ReceiveBlood )
    , mReceiveShadow( ReceiveShadow )
{
}

void RenderableComponentModifier::operator()( Opt<Actor>& Obj )
{
    IRenderableComponent& rend = *Obj->Get<IRenderableComponent>();
    rend.SetLayer( mLayer );
    rend.SetZOrder( mZOrder );
    if( mCastShadow != -1 )
    {
        rend.SetCastShadow( mCastShadow );
    }
    if( mReceiveBlood != -1 )
    {
        rend.SetReceiveBlood( mReceiveBlood );
    }
    if( mReceiveShadow != -1 )
    {
        rend.SetReceiveShadow( mReceiveShadow );
    }
}

void RenderableComponentLoader::BindValues()
{
    Bind( "zorder", func_int32_t( &RenderableComponent::SetZOrder ) );
    std::string istr;
    if( Json::GetStr( ( *mSetters )["layer"], istr ) )
    {
        Bind<RenderableLayer::Type>( &RenderableComponent::SetLayer, mRenderableLayer( AutoId( istr ) ) );
    }
    Bind( "cast_shadow", func_int32_t( &RenderableComponent::SetCastShadow ) );
    Bind( "receive_blood", func_int32_t( &RenderableComponent::SetReceiveBlood ) );
    int32_t iv;
    if( !Json::GetInt( ( *mSetters )[ "receive_shadow" ], iv ) )
    {
        iv = std::numeric_limits<int32_t>::max();
    }
    Bind<int32_t>( func_int32_t( &RenderableComponent::SetReceiveShadow ), iv );
}

RenderableComponentLoader::RenderableComponentLoader()
    : mRenderableLayer( RenderableLayer::Get() )
{
}

REAPING2_CLASS_EXPORT_IMPLEMENT( RenderableComponent, RenderableComponent );
