#include "engine/items/grenade_normal_item_sub_system.h"
#include "core/i_move_component.h"
#include "core/shot_collision_component.h"
#include "core/i_position_component.h"
#include "normal_item_sub_system.h"

namespace engine {

GrenadeNormalItemSubSystem::GrenadeNormalItemSubSystem()
    : mScene( Scene::Get() )
    , mNormalItemSubSystem(NormalItemSubSystem::Get())
    , mActorFactory(ActorFactory::Get())
    , mProjectileId( AutoId( "grenade_projectile" ) )
{
}

void GrenadeNormalItemSubSystem::Init()
{
}

void GrenadeNormalItemSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<NormalItem> normalItem = inventoryC->GetSelectedNormalItem();
    if (normalItem->IsUse())
    {
        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> Proj = mActorFactory(mProjectileId);
        Opt<IMoveComponent> projMoveC=Proj->Get<IMoveComponent>();
        Opt<IMoveComponent> actorMoveC=actor.Get<IMoveComponent>();
        Opt<IPositionComponent> projPositionC = Proj->Get<IPositionComponent>();
        Opt<IPositionComponent> actorPositionC = actor.Get<IPositionComponent>();
        Opt<ShotCollisionComponent> shotCC=Proj->Get<ShotCollisionComponent>();

        double actorOrientation = actor.Get<IPositionComponent>()->GetOrientation();

        projPositionC->SetX( actorPositionC->GetX() );
        projPositionC->SetY( actorPositionC->GetY() );
        if (shotCC.IsValid())
        {
            shotCC->SetParent( &actor );
        }
        projPositionC->SetOrientation( projPositionC->GetOrientation() + actorOrientation );
        projMoveC->SetHeading( projPositionC->GetOrientation() );

        //TODO: this is absolutely not correct. speed does not work this way in real life.
        // speed vector is not synced atm, so this is the best that i can do.
        if (actorMoveC.IsValid()&&projMoveC.IsValid())
        {
            double sumSpeedX=projMoveC->GetSpeedX()+actorMoveC->GetSpeedX()*0.8;
            double sumSpeedY=projMoveC->GetSpeedY()+actorMoveC->GetSpeedY()*0.8;
            projMoveC->SetSpeed(std::sqrt(sumSpeedX*sumSpeedX+sumSpeedY*sumSpeedY));
        }

        mScene.AddActor( Proj.release() );

        normalItem->SetConsumed(true);
    }
}

} // namespace engine

