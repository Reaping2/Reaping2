#ifndef INCLUDED_RENDER_RENDERABLE_SPRITE_H
#define INCLUDED_RENDER_RENDERABLE_SPRITE_H
#include "platform/i_platform.h"
#include "core/actor.h"
#include "sprite.h"
#include "sprite_phase.h"
#include "core/i_renderable_component.h"
#include "core/i_position_component.h"
#include "core/i_collision_component.h"

namespace render {
struct RenderableSprite
{
    Actor const* Obj;
    IRenderableComponent const* RenderableComp;
    IPositionComponent const* PositionC;
    ICollisionComponent const* CollisionC;
    int32_t ActId;
    SpritePhase const* Spr;
    SpritePhase const* MaskSpr = nullptr;
    SpritePhase const* NormalSpr = nullptr;
    Sprite const* Anim;
    std::vector<SpritePhase const*> AdditionalSprs;
    glm::vec4 Color;
    glm::vec2 RelativePosition; //relative position of the sprite to actor
    //added to the actual orientation
    double RelativeOrientation; 
    //added to the actual radius (before scale multiplication)
    double RelativeRadius;
    RenderableSprite( Actor const* o, IRenderableComponent const* rc, int32_t a, Sprite const* s, SpritePhase const* p, glm::vec4 c = glm::vec4( 1, 1, 1, 1 ) )
        : Obj( o ), RenderableComp( rc )
        , PositionC( o->Get<IPositionComponent>().Get() )
        , CollisionC( o->Get<ICollisionComponent>().Get() )
        , ActId( a ), Spr( p ), Anim( s ), Color( c )
        , RelativePosition( 0.0 )
        , RelativeOrientation(0.0)
        , RelativeRadius(0.0){}
};

} // namespace render

#endif //INCLUDED_RENDER_RENDERABLE_SPRITE_H
