#ifndef INCLUDED_RENDER_MOVE_ACTION_RENDERER_H
#define INCLUDED_RENDER_MOVE_ACTION_RENDERER_H
#include "platform/i_platform.h"
#include "core/actor.h"
#include "renderable_sprite.h"
#include "renderable_repo.h"
#include "action_renderer.h"

namespace render {

class MoveActionRenderer: public ActionRenderer
{
public:
    MoveActionRenderer( int32_t Id );
};


} // namespace render

#endif //INCLUDED_RENDER_MOVE_ACTION_RENDERER_H
