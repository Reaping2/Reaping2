#include "render/leg_move_action_renderer.h"
#include "platform/auto_id.h"
#include "engine/cloak_system.h"
namespace render {

LegMoveActionRenderer::LegMoveActionRenderer( int32_t Id )
    : ActionRenderer( Id, AutoId("leg_move") )
{

}

} // namespace render
