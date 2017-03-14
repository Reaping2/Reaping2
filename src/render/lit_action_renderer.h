#ifndef INCLUDED_RENDER_LIT_ACTION_RENDERER_H
#define INCLUDED_RENDER_LIT_ACTION_RENDERER_H

#include "platform/i_platform.h"
#include "render/action_renderer.h"
#include "core/actor.h"
#include "renderable_sprite.h"
#include "renderable_repo.h"
#include "core/i_renderable_component.h"

namespace render {

class LitActionRenderer : public ActionRenderer
{
    int32_t mLitId;
public:
    LitActionRenderer( int32_t Id );
    virtual void Init(const Actor& actor);
    virtual void FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites );
private:
};

} // namespace render

#endif//INCLUDED_RENDER_LIT_ACTION_RENDERER_H

//command:  "classgenerator.exe" -g "action_renderer" -c "lit_action_renderer"
