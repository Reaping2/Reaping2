#ifndef INCLUDED_CORE_COLLISION_MODEL_H
#define INCLUDED_CORE_COLLISION_MODEL_H
#include "core/actor.h"

class CollisionModel
{
public:
    struct Object
    {
        glm::vec2 position;
        glm::vec2 speed;
        double radius;
    };
    static Object ObjectFromActor( Actor const& actor );
    virtual bool AreActorsColliding( Object const& Obj1, Object const& Obj2, double Dt ) const = 0;
    virtual bool AreActorsColliding( Actor const& Obj1, Actor const& Obj2, double Dt ) const;
};

#endif//INCLUDED_CORE_COLLISION_MODEL_H
