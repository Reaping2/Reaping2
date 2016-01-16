#ifndef INCLUDED_RENDER_RENDERABLE_SPRITE_H
#define INCLUDED_RENDER_RENDERABLE_SPRITE_H
#include "platform/i_platform.h"
#include "core/actor.h"
#include "sprite.h"
#include "sprite_phase.h"

namespace render {
    struct RenderableSprite
    {
        Actor const* Obj;
        int32_t ActId;
        SpritePhase const* Spr;
        Sprite const* Anim;
        glm::vec4 Color;
        glm::vec2 RelativePosition; //relative position of the sprite to actor
        RenderableSprite( Actor const* o, int32_t a, Sprite const* s, SpritePhase const* p, glm::vec4 c=glm::vec4( 1,1,1,1 ) )
            : Obj( o ), ActId( a ), Anim( s ), Spr( p ), Color( c ), RelativePosition(0.0) {}
    };

} // namespace render

#endif //INCLUDED_RENDER_RENDERABLE_SPRITE_H