#include "render/switch_action_renderer.h"
#include "core/i_switch_component.h"

namespace render {


SwitchActionRenderer::SwitchActionRenderer( int32_t Id )
    : ActionRenderer(Id, AutoId( "off" ) )
{
}


void SwitchActionRenderer::Init(Actor const& actor)
{
    ActionRenderer::Init( actor );
    auto switchC( actor.Get<ISwitchComponent>() );
    if (!switchC.IsValid())
    {
        return;
    }
    SpriteCollection const& Sprites=mRenderableRepo(actor.GetId());
    UpdateState( switchC, Sprites );
}

void SwitchActionRenderer::UpdateState( Opt<ISwitchComponent> switchC, SpriteCollection const& Sprites )
{
    static auto& mSwitchState = SwitchState::Get();
    mState = switchC->GetState();
    Sprite const& Spr = Sprites( mSwitchState( mState ) );
    if (Spr.IsValid())
    {
        mSecsToEnd = Spr.GetSecsToEnd();
    }
}

void SwitchActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
{
    auto switchC( actor.Get<ISwitchComponent>() );
    if (!switchC.IsValid())
    {
        return;
    }
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    if (mState != switchC->GetState())
    {
        UpdateState( switchC, Sprites );
        if (mState == SwitchState::TransitionToOn || mState == SwitchState::TransitionToOff)
        {
            mSecsToEnd = switchC->GetSecsToEnd(); //this should be synced. This way a half way reversed transition is properly followed
        }
    }
    static auto& mSwitchState = SwitchState::Get();
    auto sprId = mSwitchState( mState );
    Sprite const& Spr = Sprites( sprId );
    auto st( (int32_t)GetState() );
    if (Spr.IsValid())
    {
        SpritePhase const& Phase = Spr( (int32_t)GetState() );
        RenderableSprite rs( &actor, &renderableC, sprId, &Spr, &Phase/*, color*/ );
        FillRenderableSprite( rs, GetCompanionSprites( actor.GetId(), sprId ), st );
        renderableSprites.push_back( rs );
    }
    
}

} // namespace render

