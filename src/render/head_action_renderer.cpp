#include "render/head_action_renderer.h"

namespace render {

HeadActionRenderer::HeadActionRenderer(int32_t Id)
    : ActionRenderer(Id)
{
    mHeadId=AutoId("head");
}


void HeadActionRenderer::Init(Actor const& actor)
{
    SpriteCollection const& Sprites=mRenderableRepo(actor.GetId());
    Sprite const& Spr=Sprites(mHeadId);
    if( Spr.IsValid() )
    {
        mSecsToEnd=Spr.GetSecsToEnd();
    }
}


void HeadActionRenderer::FillRenderableSprites(const Actor& actor, RenderableSprites_t& renderableSprites)
{
    SpriteCollection const& Sprites=mRenderableRepo(actor.GetId());
    Sprite const& Spr=Sprites(mHeadId);
    if( Spr.IsValid() )
    {
        SpritePhase const& Phase = Spr( (int32_t)GetState() );
        glm::vec4 col=GetCloakColor(actor);
        renderableSprites.push_back(
            RenderableSprite( &actor, mHeadId, &Spr, &Phase, col) );
    }
}

} // namespace render

