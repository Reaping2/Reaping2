#ifndef INCLUDED_RENDER_DEATH_ACTION_RENDERER_H
#define INCLUDED_RENDER_DEATH_ACTION_RENDERER_H
#include "platform/i_platform.h"
#include "core/actor.h"
#include "renderable_sprite.h"
#include "renderable_repo.h"
#include "action_renderer.h"

namespace render {

class DeathActionRenderer: public ActionRenderer
{
public:
    DeathActionRenderer( int32_t Id );
    virtual void Init( const Actor& actor );
    virtual void FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites );
    virtual void Update( double DeltaTime );

};


} // namespace render

#endif //INCLUDED_RENDER_DEATH_ACTION_RENDERER_H