#include "render/head_action_renderer.h"

namespace render {

HeadActionRenderer::HeadActionRenderer( int32_t Id )
    : ActionRenderer( Id, AutoId( "head" ) )
{
}

} // namespace render

