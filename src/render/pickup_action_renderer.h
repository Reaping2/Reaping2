#ifndef INCLUDED_RENDER_PICKUP_ACTION_RENDERER_H
#define INCLUDED_RENDER_PICKUP_ACTION_RENDERER_H
#include "platform/i_platform.h"
#include "core/actor.h"
#include "renderable_sprite.h"
#include "renderable_repo.h"
#include "action_renderer.h"

namespace render {

class PickupActionRenderer: public ActionRenderer
{
    int32_t mCoverId;
    int32_t mBackgroundId;
    int32_t mPickupBackgroundId = -1;
public:
    PickupActionRenderer( int32_t Id );
    virtual void Init( const Actor& actor );
    virtual void FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites );
};


} // namespace render

#endif //INCLUDED_RENDER_PICKUP_ACTION_RENDERER_H
