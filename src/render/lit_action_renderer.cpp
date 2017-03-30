#include "render/lit_action_renderer.h"
#include "../core/i_waypoint_component.h"

namespace render {

LitActionRenderer::LitActionRenderer(int32_t Id)
    : ActionRenderer(Id, AutoId( "idle" ) )
{
    mLitId=AutoId("lit");
}


void LitActionRenderer::Init(Actor const& actor)
{
    ActionRenderer::Init( actor );
    SpriteCollection const& Sprites=mRenderableRepo(actor.GetId());
    Sprite const& Spr=Sprites(mLitId);
    if( Spr.IsValid() )
    {
        mSecsToEnd=Spr.GetSecsToEnd();
    }
}

void LitActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
{
    auto waypointC( actor.Get<IWaypointComponent>() ); //TODO: there should be a lit_comp
    if (!waypointC.IsValid())
    {
        return;
    }
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    auto sprId = waypointC->IsLit() ? mLitId : mActionId;
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

