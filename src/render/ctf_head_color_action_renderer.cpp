#include "render/ctf_head_color_action_renderer.h"
#include "core/i_team_component.h"
#include "hat_action_renderer.h"

namespace render {

CtfHeadColorActionRenderer::CtfHeadColorActionRenderer( int32_t Id )
    : ActionRenderer( Id, AutoId( "ctf_head_color" ) )
{
}

glm::vec4 CtfHeadColorActionRenderer::GetRenderableColor( Actor const& actor ) const
{
    Opt<ITeamComponent> teamC( actor.Get<ITeamComponent>() );
    if ( !teamC.IsValid() )
    {
        return glm::vec4( 1,0,1,1 );
    }
    static auto& mColorRepo( ColorRepo::Get() );
    return mColorRepo( teamC->GetTeam() )*GetColor( actor )*GetCloakColor( actor );
}

void CtfHeadColorActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
{
    Opt<ITeamComponent> teamC( actor.Get<ITeamComponent>() );
    if ( !teamC.IsValid() )
    {
        return;
    }
    ActionRenderer::FillRenderableSprites( actor, renderableC, renderableSprites );
}

} // namespace render

