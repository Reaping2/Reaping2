#include "render/ctf_head_color_action_renderer.h"
#include "core/i_team_component.h"
#include "hat_action_renderer.h"

namespace render {

CtfHeadColorActionRenderer::CtfHeadColorActionRenderer( int32_t Id )
    : ActionRenderer( Id )
{
    mCtfHeadColorId = AutoId( "ctf_head_color" );
}


void CtfHeadColorActionRenderer::Init( Actor const& actor )
{
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    Sprite const& Spr = Sprites( mCtfHeadColorId );
    if( Spr.IsValid() )
    {
        mSecsToEnd = Spr.GetSecsToEnd();
    }
}


void CtfHeadColorActionRenderer::FillRenderableSprites( const Actor& actor, RenderableSprites_t& renderableSprites )
{
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    Sprite const& Spr = Sprites( mCtfHeadColorId );
    if( Spr.IsValid() )
    {
        Opt<ITeamComponent> teamC( actor.Get<ITeamComponent>() );
        if ( teamC.IsValid() )
        {
            SpritePhase const& Phase = Spr( ( int32_t )GetState() );
            glm::vec4 col = ColorRepo::Get()( teamC->GetTeam() );
            col.a = GetCloakColor( actor ).a;
            renderableSprites.push_back(
                RenderableSprite( &actor, mCtfHeadColorId, &Spr, &Phase, col ) );
        }
    }
}

} // namespace render

