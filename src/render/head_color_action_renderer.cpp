#include "render/head_color_action_renderer.h"
#include "core/player_controller_component.h"
#include "hat_action_renderer.h"

namespace render {

HeadColorActionRenderer::HeadColorActionRenderer( int32_t Id )
    : ActionRenderer( Id, AutoId( "head_color" ) )
{
}

glm::vec4 HeadColorActionRenderer::GetRenderableColor( Actor const& actor ) const
{
    Opt<PlayerControllerComponent> playerCC = actor.Get<IControllerComponent>();
    glm::vec4 col = (playerCC.IsValid() ? ColorRepo::Get()(playerCC->mControllerId) : glm::vec4( 1, 1, 1, 1 ))*GetColor( actor );
    col.a = GetCloakColor( actor ).a;
    col = col * GetColor( actor );
    return col;
}


} // namespace render

