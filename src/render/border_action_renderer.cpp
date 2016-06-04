#include "render/border_action_renderer.h"
#include "core/border_type.h"
#include "core/i_border_component.h"
#include "platform/id_storage.h"
#include "core/i_collision_component.h"

namespace render {

BorderActionRenderer::BorderActionRenderer( int32_t Id )
    : ActionRenderer( Id )
    , mActionId( AutoId( "body_idle" ) )
    , mActorSize( 1.0 )
    , mBorderType( BorderType::Get() )
{
}


void BorderActionRenderer::Init( Actor const& actor )
{
    static BorderType& mBorderType = BorderType::Get();
    static IdStorage& mIdStorage = IdStorage::Get();
    Opt<IBorderComponent> borderC = actor.Get<IBorderComponent>();
    if ( !borderC.IsValid() )
    {
        // this should be valid. border_recognizer guarantees it. Other recognizers can be paired with this renderer
        BOOST_ASSERT( borderC.IsValid() );
        return;
    }
    std::string actorName;
    bool const gotId = mIdStorage.GetName( actor.GetId(), actorName );
    BOOST_ASSERT( gotId );
    mBorders = borderC->GetBorders();
    for ( IBorderComponent::Borders_t::iterator i = mBorders.begin(), e = mBorders.end(); i != e; ++i )
    {
        std::string borderName;
        if( mIdStorage.GetName( mBorderType( *i ), borderName ) )
        {
            mBorderIds.push_back( mIdStorage.GetId( actorName + "_" + borderName ) );
        }
    }
    mOuterBorders = borderC->GetOuterBorders();
    for ( IBorderComponent::Borders_t::iterator i = mOuterBorders.begin(), e = mOuterBorders.end(); i != e; ++i )
    {
        std::string borderName;
        if( mIdStorage.GetName( mBorderType( *i ), borderName ) )
        {
            mOuterBorderIds.push_back( mIdStorage.GetId( actorName + "_" + borderName + "_outer" ) );
        }
    }
    double scale = 1.0;
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    Sprite const& Spr = Sprites( mActionId );
    if( Spr.IsValid() )
    {
        scale = Spr.GetScale() * 1.0;
    }
    Opt<ICollisionComponent> const collisionC = actor.Get<ICollisionComponent>();
    mActorSize = ( ( GLfloat )( ( collisionC.IsValid() ? collisionC->GetRadius() : 50 ) * scale ) );

    for ( BorderIds_t::const_iterator i = mBorderIds.begin(), e = mBorderIds.end(); i != e; ++i )
    {
        SpriteCollection const& Sprites = mRenderableRepo( *i );
        Sprite const& Spr = Sprites( mActionId );
        if( Spr.IsValid() )
        {
            mSprites.emplace_back( glm::vec2(), Spr );
        }
    }
    IBorderComponent::Borders_t::const_iterator outer_i = mOuterBorders.begin();
    for ( BorderIds_t::const_iterator i = mOuterBorderIds.begin(), e = mOuterBorderIds.end(); i != e; ++i, ++outer_i )
    {
        SpriteCollection const& Sprites = mRenderableRepo( *i );
        Sprite const& Spr = Sprites( mActionId );
        if( Spr.IsValid() )
        {
            glm::vec2 pos = mBorderType.GetNeighborDirs()[*outer_i];
            mSprites.emplace_back( glm::vec2( 2 * pos.x * mActorSize, 2 * pos.y * mActorSize ), Spr );
        }
    }
}


void BorderActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
{
    for( auto const& data : mSprites )
    {
        SpritePhase const& Phase = data.Spr( ( int32_t )GetState() );
        RenderableSprite renderableSprite( &actor, &renderableC, mActionId, &data.Spr, &Phase/*, color*/ );
        renderableSprite.RelativePosition = data.RelativePosition;
        renderableSprites.push_back( renderableSprite );
    }
}

} // namespace render

