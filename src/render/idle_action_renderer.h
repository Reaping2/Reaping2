#ifndef INCLUDED_RENDER_IDLE_ACTION_RENDERER_H
#define INCLUDED_RENDER_IDLE_ACTION_RENDERER_H
#include "platform/i_platform.h"
#include "core/actor.h"
#include "renderable_sprite.h"
#include "renderable_repo.h"
#include "action_renderer.h"

namespace render {

    class IdleActionRenderer: public ActionRenderer
    {
    public:
        IdleActionRenderer( int32_t Id );
        virtual void Init(const Actor& actor);
        virtual void FillRenderableSprites(const Actor& actor, RenderableSprites_t& renderableSprites);
    };


} // namespace render

#endif //INCLUDED_RENDER_IDLE_ACTION_RENDERER_H