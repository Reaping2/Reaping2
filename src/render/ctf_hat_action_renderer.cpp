#include "render/ctf_hat_action_renderer.h"
#include "core/ctf_program_state.h"
#include "core/player_controller_component.h"
#include "core/i_team_component.h"

namespace render {
namespace ctf {

CtfHatActionRenderer::CtfHatActionRenderer( int32_t Id )
    : ActionRenderer( Id, AutoId( "ctf_body_color" ) )
    , mColorRepo( ColorRepo::Get() )
{
}

glm::vec4 CtfHatActionRenderer::GetRenderableColor( Actor const& actor ) const
{
    Opt<ITeamComponent> teamC( actor.Get<ITeamComponent>() );
    if ( !teamC.IsValid() )
    {
        return glm::vec4( 1,0,1,1 );
    }
    return mColorRepo( teamC->GetTeam() )*GetColor( actor )*GetCloakColor( actor );
}

void CtfHatActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
{
    Opt<ITeamComponent> teamC( actor.Get<ITeamComponent>() );
    if ( !teamC.IsValid() )
    {
        return;
    }
    ActionRenderer::FillRenderableSprites( actor, renderableC, renderableSprites );
}

} // namespace ctf
} // namespace render

