#ifndef INCLUDED_CORE_BOX_COLLISION_MODEL_H
#define INCLUDED_CORE_BOX_COLLISION_MODEL_H

class BoxCollisionModel : public CollisionModel
{
    bool AreActorsColliding( Actor const& Obj1, Actor const& Obj2, double Dt )const;
};

#endif//INCLUDED_CORE_BOX_COLLISION_MODEL_H
