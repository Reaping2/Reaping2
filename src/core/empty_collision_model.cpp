#include "i_core.h"
#include "core/empty_collision_model.h"

bool EmptyCollisionModel::AreActorsColliding( Actor const& Obj1, Actor const& Obj2, double Dt )const
{
    return false;
}
