#include "platform/i_platform.h"
#include "engine/explode_on_death_system.h"
#include "core/i_explode_on_death_component.h"
#include "core/i_health_component.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/i_collision_component.h"
#include "items/weapon_item_sub_system.h"
#include "core/i_owner_component.h"

namespace engine {

ExplodeOnDeathSystem::ExplodeOnDeathSystem()
    : mScene( Scene::Get() )
    , mActorFactory(ActorFactory::Get())
{

}

void ExplodeOnDeathSystem::Init()
{
}

void ExplodeOnDeathSystem::Update(double DeltaTime)
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(), n; ( n = it, it != e ? ( ++n, true ) : false ); it = n )
    {
        Actor& actor = **it;
        Opt<IExplodeOnDeathComponent> explodeOnDeathC = actor.Get<IExplodeOnDeathComponent>();
        if (!explodeOnDeathC.IsValid())
        {
            continue;
        }
        Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
        if(!healthC.IsValid())
        {
            continue; 
        }
        if(!healthC->IsAlive())
        {
            AddExplosionProjectiles(*explodeOnDeathC.Get(), actor);

            mScene.RemoveActor(it);
        }
    }
}

void ExplodeOnDeathSystem::AddExplosionProjectiles(IExplode& explode, Actor &actor)
{
    WeaponItemSubSystem::Projectiles_t projectiles;

    std::auto_ptr<Actor> ps;
    static ActorFactory& mActorFactory=ActorFactory::Get();
    for (int i=0;i<explode.GetCount();++i)
    {
        ps = mActorFactory(explode.GetExplosionProjectile());
        Opt<IOwnerComponent> actorOwnerC=actor.Get<IOwnerComponent>();
        Opt<IOwnerComponent> psOwnerC=ps->Get<IOwnerComponent>();
        if (actorOwnerC.IsValid()&&psOwnerC.IsValid())
        {
            psOwnerC->SetOwnerGUID(actorOwnerC->GetOwnerGUID());
        }
        if (explode.GetCount()>1)
        {
            ps->Get<IPositionComponent>()->SetOrientation( -1*explode.GetScatter() + i*
                (explode.GetScatter()/(explode.GetCount()-1)*2) );
        }
        projectiles.push_back( Opt<Actor>(ps.release()) );
    }
    Scatter scatter;
    WeaponItemSubSystem::Get()->AddProjectiles(actor,projectiles,scatter);
}



} // namespace engine

