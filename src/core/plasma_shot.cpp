#include "i_core.h"
#include "core/shot_collision_component.h"

PlasmaShot::PlasmaShot()
    : Shot( "plasma_shot" )
{
    Get<ICollisionComponent>()->SetRadius(0.02);
    Get<ShotCollisionComponent>()->SetDamage(30);
}
