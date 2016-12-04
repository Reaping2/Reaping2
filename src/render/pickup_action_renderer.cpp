#include "render/pickup_action_renderer.h"
#include "platform/auto_id.h"
#include "core/pickup_collision_component.h"
#include "core/i_pulse_component.h"
namespace render {


PickupActionRenderer::PickupActionRenderer( int32_t Id )
    : ActionRenderer( Id )
{
    mActionId = AutoId( "body_idle" );
}

void PickupActionRenderer::Init( const Actor& actor )
{
    Opt<PickupCollisionComponent> pickupCC( actor.Get<ICollisionComponent>() );
    int32_t pickupContent = pickupCC->GetPickupContent();
    SpriteCollection const& Sprites = mRenderableRepo( pickupContent );
    Sprite const& Spr = Sprites( mActionId );
    if( Spr.IsValid() )
    {
        mSecsToEnd = Spr.GetSecsToEnd();
    }
}

void PickupActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
{
    Opt<PickupCollisionComponent> pickupCC( actor.Get<ICollisionComponent>() );
    auto pulseC( actor.Get<IPulseComponent>() );
    static auto& mItemColorRepo( ItemColorRepo::Get() );
    auto col = GetColor( actor );
    {
        static const int32_t mPickupBackgroundId = AutoId( "pickup_background" );
        SpriteCollection const& Sprites = mRenderableRepo( mPickupBackgroundId );
        Sprite const& Spr = Sprites( mActionId );
        if (Spr.IsValid())
        {
            SpritePhase const& Phase = Spr( (int32_t)GetState() );
            auto renderableSprite = RenderableSprite( &actor, &renderableC, mActionId, &Spr, &Phase, col*mItemColorRepo( pickupCC->GetItemType() ) );
            if (pulseC.IsValid())
            {
                auto& pulses = pulseC->GetPulses();
                if (pulses.size() > 0)
                {
                    auto& backSizePulse = pulses[0];
                    renderableSprite.RelativeRadius = backSizePulse.mSpeed * (backSizePulse.mDurationCurrent - backSizePulse.mDuration / 2);
                }
            }
            renderableSprites.push_back( renderableSprite );
        }
    }
    {
        int32_t pickupContent = pickupCC->GetPickupContent();
        SpriteCollection const& Sprites = mRenderableRepo( pickupContent );
        Sprite const& Spr = Sprites( mActionId );
        if (Spr.IsValid())
        {
            SpritePhase const& Phase = Spr( (int32_t)GetState() );
            auto renderableSprite = RenderableSprite( &actor, &renderableC, mActionId, &Spr, &Phase, col );
            if (pulseC.IsValid())
            {
                auto& pulses = pulseC->GetPulses();
                if (pulses.size() > 1)
                {
                    auto& backSizePulse = pulses[1];
                    renderableSprite.RelativeRadius = backSizePulse.mSpeed * (backSizePulse.mDurationCurrent - backSizePulse.mDuration / 2);
                }
                if (pulses.size() > 2)
                {
                    auto& backSizePulse = pulses[2];
                    renderableSprite.RelativeOrientation = backSizePulse.mSpeed * (backSizePulse.mDurationCurrent - backSizePulse.mDuration / 2);
                }
            }
            renderableSprites.push_back( renderableSprite );
        }
    }
}

} // namespace render
