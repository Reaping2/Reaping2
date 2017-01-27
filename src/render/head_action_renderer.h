#ifndef INCLUDED_RENDER_HEAD_ACTION_RENDERER_H
#define INCLUDED_RENDER_HEAD_ACTION_RENDERER_H

#include "platform/i_platform.h"
#include "render/action_renderer.h"
#include "core/actor.h"
#include "renderable_sprite.h"
#include "renderable_repo.h"

namespace render {

class HeadActionRenderer : public ActionRenderer
{
public:
    HeadActionRenderer( int32_t Id );
};

} // namespace render

#endif//INCLUDED_RENDER_HEAD_ACTION_RENDERER_H

//command:  "classgenerator.exe" -g "action_renderer" -c "head_action_renderer"
