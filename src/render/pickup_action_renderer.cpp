#include "render/pickup_action_renderer.h"
#include "platform/auto_id.h"
#include "core/pickup_collision_component.h"
namespace render {


PickupActionRenderer::PickupActionRenderer( int32_t Id )
    : ActionRenderer( Id )
{
    mActionId = AutoId( "body_idle" );
}

void PickupActionRenderer::Init( const Actor& actor )
{
    int32_t pickupContent = actor.Get<PickupCollisionComponent>()->GetPickupContent();
    SpriteCollection const& Sprites = mRenderableRepo( pickupContent );
    Sprite const& Spr = Sprites( mActionId );
    if( Spr.IsValid() )
    {
        mSecsToEnd = Spr.GetSecsToEnd();
    }
}

void PickupActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
{
    int32_t pickupContent = actor.Get<PickupCollisionComponent>()->GetPickupContent();
    SpriteCollection const& Sprites = mRenderableRepo( pickupContent );
    Sprite const& Spr = Sprites( mActionId );
    if( Spr.IsValid() )
    {
        SpritePhase const& Phase = Spr( ( int32_t )GetState() );
        renderableSprites.push_back( RenderableSprite( &actor, &renderableC, mActionId, &Spr, &Phase ) );
    }
}

} // namespace render