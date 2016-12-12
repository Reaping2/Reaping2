#ifndef INCLUDED_CORE_BOX_COLLISION_MODEL_H
#define INCLUDED_CORE_BOX_COLLISION_MODEL_H
#include "core/collision_model.h"
#include "core/actor.h"

class BoxCollisionModel : public CollisionModel
{
public:
    using CollisionModel::AreActorsColliding;
    static bool AreActorsColliding_static( Object const& ObjA, Object const& ObjB, double Dt );
    bool AreActorsColliding( Object const& ObjA, Object const& ObjB, double Dt ) const;
};

#endif//INCLUDED_CORE_BOX_COLLISION_MODEL_H
