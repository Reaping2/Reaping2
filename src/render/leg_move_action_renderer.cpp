#include "render/leg_move_action_renderer.h"
#include "platform/auto_id.h"
namespace render{


    LegMoveActionRenderer::LegMoveActionRenderer(int32_t Id)
        : ActionRenderer(Id)
    {

    }

    void LegMoveActionRenderer::Init(const Actor& actor)
    {
        SpriteCollection const& Sprites=mRenderableRepo(actor.GetId());
        int32_t aid=AutoId("move");
        Sprite const& Spr=Sprites(aid);
        if( Spr.IsValid() )
        {
            mSecsToEnd=Spr.GetSecsToEnd();
        }
    }

    void LegMoveActionRenderer::FillRenderableSprites(const Actor& actor, RenderableSprites_t& renderableSprites)
    {
        SpriteCollection const& Sprites=mRenderableRepo(actor.GetId());
        int32_t aid=AutoId("move");
        Sprite const& Spr=Sprites(aid);
        if( Spr.IsValid() )
        {
            SpritePhase const& Phase = Spr( (int32_t)GetState() );
            renderableSprites.push_back( RenderableSprite( &actor, aid, &Spr, &Phase ) );
        }
    }

} // namespace render