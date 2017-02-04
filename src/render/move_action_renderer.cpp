#include "render/move_action_renderer.h"
#include "platform/auto_id.h"
namespace render {

MoveActionRenderer::MoveActionRenderer( int32_t Id )
    : ActionRenderer( Id, AutoId( "move" ) )
{

}

} // namespace render
