#ifndef INCLUDED_RENDER_CELL_ACTION_RENDERER_H
#define INCLUDED_RENDER_CELL_ACTION_RENDERER_H

#include "platform/i_platform.h"
#include "render/action_renderer.h"
#include "core/actor.h"
#include "renderable_sprite.h"
#include "renderable_repo.h"

namespace render {

class CellActionRenderer : public ActionRenderer
{
    int32_t mCellId;
public:
    CellActionRenderer( int32_t Id );
    virtual void FillRenderableSprites(const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites);
private:
};

} // namespace render

#endif//INCLUDED_RENDER_CELL_ACTION_RENDERER_H

//command:  "classgenerator.exe" -g "action_renderer" -c "cell_action_renderer"
