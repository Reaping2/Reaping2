#ifndef INCLUDED_CORE_EMPTY_COLLISION_MODEL_H
#define INCLUDED_CORE_EMPTY_COLLISION_MODEL_H
#include "core/collision_model.h"

class EmptyCollisionModel : public CollisionModel
{
    bool AreActorsColliding( Object const& Obj1, Object const& Obj2, double Dt ) const;
    bool AreActorsColliding( Actor const& Obj1, Actor const& Obj2, double Dt )const;
};

#endif//INCLUDED_CORE_EMPTY_COLLISION_MODEL_H
