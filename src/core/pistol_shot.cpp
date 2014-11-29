#include "platform/i_platform.h"
#include "core/pistol_shot.h"
#include "core/i_collision_component.h"
#include "core/shot_collision_component.h"

PistolShot::PistolShot()
    : Shot( "pistol_shot" )
{
    Get<ICollisionComponent>()->SetRadius(0.01);
    Get<ShotCollisionComponent>()->SetDamage(10);
}

