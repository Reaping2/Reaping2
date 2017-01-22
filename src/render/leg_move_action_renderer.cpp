#include "render/leg_move_action_renderer.h"
#include "platform/auto_id.h"
#include "engine/cloak_system.h"
namespace render {


LegMoveActionRenderer::LegMoveActionRenderer( int32_t Id )
    : ActionRenderer( Id )
{

}

void LegMoveActionRenderer::Init( const Actor& actor )
{
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    static int32_t aid = AutoId( "leg_move" );
    Sprite const& Spr = Sprites( aid );
    if( Spr.IsValid() )
    {
        mSecsToEnd = Spr.GetSecsToEnd();
    }
}

void LegMoveActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
{
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    static int32_t aid = AutoId( "leg_move" );
    Sprite const& Spr = Sprites( aid );
    if( Spr.IsValid() )
    {
        SpritePhase const& Phase = Spr( ( int32_t )GetState() );
        glm::vec4 col = GetCloakColor( actor ) * GetColor( actor );
        renderableSprites.push_back( RenderableSprite( &actor, &renderableC, aid, &Spr, &Phase, col ) );
    }
}

} // namespace render
