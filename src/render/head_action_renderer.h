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
    int32_t mHeadId;
public:
    HeadActionRenderer( int32_t Id );
    virtual void Init(const Actor& actor);
    virtual void FillRenderableSprites(const Actor& actor, RenderableSprites_t& renderableSprites);
private:
};

} // namespace render

#endif//INCLUDED_RENDER_HEAD_ACTION_RENDERER_H

//command:  "classgenerator.exe" -g "action_renderer" -c "head_action_renderer"
