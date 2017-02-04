#ifndef INCLUDED_RENDER_LEG_MOVE_ACTION_RENDERER_H
#define INCLUDED_RENDER_LEG_MOVE_ACTION_RENDERER_H
#include "platform/i_platform.h"
#include "core/actor.h"
#include "renderable_sprite.h"
#include "renderable_repo.h"
#include "action_renderer.h"

namespace render {

class LegMoveActionRenderer: public ActionRenderer
{
public:
    LegMoveActionRenderer( int32_t Id );
};


} // namespace render

#endif //INCLUDED_RENDER_LEG_MOVE_ACTION_RENDERER_H
