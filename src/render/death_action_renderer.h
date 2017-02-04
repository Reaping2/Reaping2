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
    virtual void Update( double DeltaTime );

};


} // namespace render

#endif //INCLUDED_RENDER_DEATH_ACTION_RENDERER_H
