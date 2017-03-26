#include "platform/i_platform.h"
#include "engine/engine.h"
#include "engine/collisions/dark_matter_collision_sub_system.h"
#include "core/dark_matter_collision_component.h"
#include "core/i_inventory_component.h"
#include "core/i_health_component.h"

namespace engine {

DarkMatterCollisionSubSystem::DarkMatterCollisionSubSystem()
    : CollisionSubSystem()
{
}


void DarkMatterCollisionSubSystem::Init()
{
}


void DarkMatterCollisionSubSystem::Update(Actor& actor, double DeltaTime)
{
}


void DarkMatterCollisionSubSystem::Collide( Actor& actor, Actor& other )
{
    Opt<DarkMatterCollisionComponent> darkMatterCC = actor.Get<ICollisionComponent>();
    Opt<IInventoryComponent> inventoryC = other.Get<IInventoryComponent>();
    if (inventoryC.IsValid())
    {
        Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
        if (healthC.IsValid()&&healthC->GetHP()>0)
        {
            if (inventoryC->IsCollectDarkMatter())
            {
                inventoryC->SetDarkMatters( inventoryC->GetDarkMatters() + 1 );
            }
            healthC->SetHP( 0 );
        }
    }
}

void DarkMatterCollisionSubSystem::ClipScene( Actor& actor )
{
    CollisionSubSystem::ClipScene(actor);
    Opt<DarkMatterCollisionComponent> darkMatterCC=actor.Get<ICollisionComponent>();
}


} // namespace engine

