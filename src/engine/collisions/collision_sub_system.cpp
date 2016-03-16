#include "platform/i_platform.h"
#include "engine/collisions/collision_sub_system.h"
#include "core/i_collision_component.h"
#include "core/i_position_component.h"

namespace engine {

CollisionSubSystem::CollisionSubSystem()
    : mScene( Scene::Get() )
{
}

void CollisionSubSystem::ClipScene( Actor& actor )
{
    Opt<ICollisionComponent> collisionC = actor.Get<ICollisionComponent>();
    Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
    if (!collisionC->IsClipScene())
    {
        return;
    }

    glm::vec4 AllowedDimensions = mScene.GetDimensions();
    float Radius = ( float )collisionC->GetRadius();
    AllowedDimensions.x += Radius;
    AllowedDimensions.y += Radius;
    AllowedDimensions.z -= Radius;
    AllowedDimensions.w -= Radius;
    if( positionC->GetX() < AllowedDimensions.x )
    {
        positionC->SetX( AllowedDimensions.x );
    }
    else if( positionC->GetX() > AllowedDimensions.z )
    {
        positionC->SetX( AllowedDimensions.z );
    }
    if( positionC->GetY() < AllowedDimensions.y )
    {
        positionC->SetY( AllowedDimensions.y );
    }
    else if( positionC->GetY() > AllowedDimensions.w )
    {
        positionC->SetY( AllowedDimensions.w );
    }
}

void CollisionSubSystem::Collide( Actor& actor, Actor& other )
{

}

} // namespace engine

