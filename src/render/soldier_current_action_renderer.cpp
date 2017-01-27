#include "render/soldier_current_action_renderer.h"
#include "core/ctf_program_state.h"
#include "core/player_controller_component.h"
#include "core/i_team_component.h"
#include "core/program_state.h"

namespace render {
namespace ctf {

SoldierCurrentActionRenderer::SoldierCurrentActionRenderer( int32_t Id )
    : ActionRenderer( Id, AutoId( "soldier_current" ) )
    , mColorRepo( ColorRepo::Get() )
    , mColor( 0.29, 0.05, 0.4, 0.8 )
{
}

glm::vec4 SoldierCurrentActionRenderer::GetRenderableColor( Actor const& actor ) const
{
    return mColor;
}

void SoldierCurrentActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
{
    if ( actor.GetGUID() == core::ProgramState::Get().mControlledActorGUID )
    {
        ActionRenderer::FillRenderableSprites( actor, renderableC, renderableSprites );
    }
}

} // namespace ctf
} // namespace render

