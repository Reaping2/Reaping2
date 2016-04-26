#include "render/ctf_flag_action_renderer.h"
#include "core/i_team_component.h"

namespace render {
namespace ctf {

CtfFlagActionRenderer::CtfFlagActionRenderer( int32_t Id )
    : ActionRenderer( Id )
    , mColorRepo( ColorRepo::Get() )
{
    mCtfFlagId = AutoId( "body_idle" );
}


void CtfFlagActionRenderer::Init( Actor const& actor )
{
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    Sprite const& Spr = Sprites( mCtfFlagId );
    if( Spr.IsValid() )
    {
        mSecsToEnd = Spr.GetSecsToEnd();
    }
}


void CtfFlagActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
{
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    Sprite const& Spr = Sprites( mCtfFlagId );
    if( Spr.IsValid() )
    {
        Opt<ITeamComponent> teamC( actor.Get<ITeamComponent>() );
        if ( teamC.IsValid() )
        {
            SpritePhase const& Phase = Spr( ( int32_t )GetState() );
            renderableSprites.push_back(
                RenderableSprite( &actor, &renderableC, mCtfFlagId, &Spr, &Phase,  mColorRepo( teamC->GetTeam() ) ) );
        }
    }
}

} // namespace ctf
} // namespace render

