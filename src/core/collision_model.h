#ifndef INCLUDED_CORE_COLLISION_MODEL_H
#define INCLUDED_CORE_COLLISION_MODEL_H

class CollisionModel
{
public:
    virtual bool AreActorsColliding( Actor const& Obj1, Actor const& Obj2, double Dt )const = 0;
};

#endif//INCLUDED_CORE_COLLISION_MODEL_H
