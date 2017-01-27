#include "render/ctf_flag_action_renderer.h"
#include "core/i_team_component.h"

namespace render {
namespace ctf {

CtfFlagActionRenderer::CtfFlagActionRenderer( int32_t Id )
    : ActionRenderer( Id, AutoId( "idle" ) )
    , mColorRepo( ColorRepo::Get() )
{
}

glm::vec4 CtfFlagActionRenderer::GetRenderableColor( Actor const& actor ) const
{
    Opt<ITeamComponent> teamC( actor.Get<ITeamComponent>() );
    if ( !teamC.IsValid() )
    {
        return glm::vec4( 1,0,1,1 );
    }
    return mColorRepo( teamC->GetTeam() )*GetColor( actor );
}

void CtfFlagActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
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

