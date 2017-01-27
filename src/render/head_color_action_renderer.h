#ifndef INCLUDED_RENDER_HEAD_COLOR_ACTION_RENDERER_H
#define INCLUDED_RENDER_HEAD_COLOR_ACTION_RENDERER_H

#include "platform/i_platform.h"
#include "render/action_renderer.h"
#include "core/actor.h"
#include "renderable_sprite.h"
#include "renderable_repo.h"

namespace render {

class HeadColorActionRenderer : public ActionRenderer
{
public:
    HeadColorActionRenderer( int32_t Id );
protected:
    virtual glm::vec4 GetRenderableColor( Actor const& actor ) const;
};

} // namespace render

#endif//INCLUDED_RENDER_HEAD_COLOR_ACTION_RENDERER_H

//command:  "classgenerator.exe" -g "action_renderer" -c "head_color_action_renderer"
