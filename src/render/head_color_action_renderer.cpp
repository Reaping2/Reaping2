#include "render/head_color_action_renderer.h"
#include "core/player_controller_component.h"
#include "hat_action_renderer.h"

namespace render {

HeadColorActionRenderer::HeadColorActionRenderer( int32_t Id )
    : ActionRenderer( Id )
{
    mHeadColorId = AutoId( "head_color" );
}


void HeadColorActionRenderer::Init( Actor const& actor )
{
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    Sprite const& Spr = Sprites( mHeadColorId );
    if( Spr.IsValid() )
    {
        mSecsToEnd = Spr.GetSecsToEnd();
    }
}


void HeadColorActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
{
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    Sprite const& Spr = Sprites( mHeadColorId );
    if( Spr.IsValid() )
    {
        SpritePhase const& Phase = Spr( ( int32_t )GetState() );
        Opt<PlayerControllerComponent> playerCC = actor.Get<PlayerControllerComponent>();
        glm::vec4 col = ColorRepo::Get()( playerCC->mControllerId )*GetColor( actor );
        col.a = GetCloakColor( actor ).a;
        renderableSprites.push_back(
            RenderableSprite( &actor, &renderableC, mHeadColorId, &Spr, &Phase, col ) );
    }
}

} // namespace render

