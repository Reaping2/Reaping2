#include "render/idle_action_renderer.h"
#include "platform/auto_id.h"

namespace render {

IdleActionRenderer::IdleActionRenderer( int32_t Id )
    : ActionRenderer( Id, AutoId( "idle" ) )
{
}

} // namespace render
