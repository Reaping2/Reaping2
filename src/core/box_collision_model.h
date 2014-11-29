#ifndef INCLUDED_CORE_BOX_COLLISION_MODEL_H
#define INCLUDED_CORE_BOX_COLLISION_MODEL_H
#include "core/collision_model.h"
#include "core/actor.h"

class BoxCollisionModel : public CollisionModel
{
    bool AreActorsColliding( Actor const& Obj1, Actor const& Obj2, double Dt )const;
};

#endif//INCLUDED_CORE_BOX_COLLISION_MODEL_H
