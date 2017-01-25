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
    Sprite const* mSpr = nullptr;
    Sprite const* mJointSpr = nullptr;
    Sprite const* mMaskSpr = nullptr;
public:
    IdleActionRenderer( int32_t Id );
    virtual void Init( const Actor& actor );

    static int32_t GetSpriteId( int32_t spriteIndex, int32_t actorId );
    static std::pair<Sprite const*, Sprite const*> GetJointAndMaskSprites( int32_t actorId, int32_t actionId );

    virtual void FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites );
};


} // namespace render

#endif //INCLUDED_RENDER_IDLE_ACTION_RENDERER_H
