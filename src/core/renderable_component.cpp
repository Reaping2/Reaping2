#include "core/renderable_component.h"
#include "core/actor.h"
#include "platform/i_platform.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
#include <limits>

RenderableComponent::RenderableComponent()
    : mLayerPriority( 0 )
    , mZOrder( 0 )
    , mCastShadow( 0 )
    , mReceiveBlood( 0 )
    , mReceiveShadow( 0 )
    , mColor( glm::vec4(1.0) )
    , mRandomSprites( )
    , mSpriteIndex( -1 )
    , mShaderId( -1 )
{
}

int32_t const& RenderableComponent::GetLayerPriority() const
{
    return mLayerPriority;
}

void RenderableComponent::SetLayerPriority( int32_t Lay )
{
    mLayerPriority = Lay;
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

void RenderableComponent::SetColor( glm::vec4 const& color )
{
    mColor = color;
}

glm::vec4 RenderableComponent::GetColor()const
{
    return mColor;
}

void RenderableComponent::SetRandomSprites( RandomSprites_t const& randomSprites )
{
    mRandomSprites = randomSprites;
}

IRenderableComponent::RandomSprites_t const& RenderableComponent::GetRandomSprites() const
{
    return mRandomSprites;
}

void RenderableComponent::SetSpriteIndex( int32_t spriteIndex )
{
    mSpriteIndex = spriteIndex;
}

int32_t RenderableComponent::GetSpriteIndex() const
{
    return mSpriteIndex;
}

void RenderableComponent::SetShaderId( int32_t id )
{
    mShaderId = id;
}

int32_t RenderableComponent::GetShaderId() const
{
    return mShaderId;
}

void RenderableComponent::SetPostProcessIds( std::vector<int32_t> const& ids )
{
    mPostprocessorIds = ids;
}

std::vector<int32_t>const& RenderableComponent::GetPostProcessIds() const
{
    return mPostprocessorIds;
}

RenderableComponentModifier::RenderableComponentModifier( int32_t Lay, int32_t ZOrder, int32_t CastShadow, int32_t ReceiveBlood, int32_t ReceiveShadow )
    : mLayerPriority( Lay )
    , mZOrder( ZOrder )
    , mCastShadow( CastShadow )
    , mReceiveBlood( ReceiveBlood )
    , mReceiveShadow( ReceiveShadow )
{
}

void RenderableComponentModifier::operator()( Opt<Actor>& Obj )
{
    IRenderableComponent& rend = *Obj->Get<IRenderableComponent>();
    rend.SetLayerPriority( mLayerPriority );
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
        Bind<int32_t>( func_int32_t(&RenderableComponent::SetLayerPriority), mRenderableLayer( istr ) );
    }
    if( Json::GetStr( ( *mSetters )["shader_id"], istr ) )
    {
        Bind<int32_t>( &RenderableComponent::SetShaderId, AutoId( istr ) );
    }
    Bind( "cast_shadow", func_int32_t( &RenderableComponent::SetCastShadow ) );
    Bind( "receive_blood", func_int32_t( &RenderableComponent::SetReceiveBlood ) );
    int32_t iv;
    if( !Json::GetInt( ( *mSetters )[ "receive_shadow" ], iv ) )
    {
        iv = std::numeric_limits<int32_t>::max();
    }
    Bind<int32_t>( func_int32_t( &RenderableComponent::SetReceiveShadow ), iv );
    Bind( "color", func_color( &RenderableComponent::SetColor ) );

    IRenderableComponent::RandomSprites_t randomSprites;
    auto const& json = (*mSetters)["random_sprites"];
    if (json.isArray())
    {
        for (auto& chance : json)
        {
            randomSprites.push_back( chance.asInt() );
        }
        Bind<IRenderableComponent::RandomSprites_t>( &RenderableComponent::SetRandomSprites, randomSprites );
    }
    auto const& postprocessors = (*mSetters)["postprocessors"];
    if (postprocessors.isArray())
    {
        std::vector<int32_t> ids;
        for( auto const& proc : postprocessors )
        {
            std::string procname;
            if( Json::GetStr( proc, procname ) )
            {
                ids.push_back( AutoId( procname ) );
            }
        }
        Bind<std::vector<int32_t> >( &RenderableComponent::SetPostProcessIds, ids );
    }
}

RenderableComponentLoader::RenderableComponentLoader()
    : mRenderableLayer( RenderableLayer::Get() )
{
}

REAPING2_CLASS_EXPORT_IMPLEMENT( RenderableComponent, RenderableComponent );
