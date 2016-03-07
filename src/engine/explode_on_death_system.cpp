#include "platform/i_platform.h"
#include "engine/explode_on_death_system.h"
#include "core/i_explode_on_death_component.h"
#include "core/i_health_component.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/i_collision_component.h"
#include "items/weapon_item_sub_system.h"
#include "core/i_owner_component.h"
#include "core/explode_distribution_type.h"
#include "core/i_fade_out_component.h"

namespace engine {

ExplodeOnDeathSystem::ExplodeOnDeathSystem()
    : mScene( Scene::Get() )
    , mActorFactory( ActorFactory::Get() )
{

}

void ExplodeOnDeathSystem::Init()
{
}

void ExplodeOnDeathSystem::Update( double DeltaTime )
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(), n; ( n = it, it != e ? ( ++n, true ) : false ); it = n )
    {
        Actor& actor = **it;
        Opt<IExplodeOnDeathComponent> explodeOnDeathC = actor.Get<IExplodeOnDeathComponent>();
        if ( !explodeOnDeathC.IsValid() )
        {
            continue;
        }
        Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
        if( !healthC.IsValid() )
        {
            continue;
        }
        if( !healthC->IsAlive() )
        {
            WeaponItemSubSystem::Projectiles_t projectiles;
            FillExplosionProjectiles( *explodeOnDeathC.Get(), actor, projectiles );
            Scatter scatter;
            WeaponItemSubSystem::Get()->AddProjectiles( actor, projectiles, scatter );
            mScene.RemoveActor( it );
        }
    }
}

void ExplodeOnDeathSystem::FillExplosionProjectiles( IExplode& explode, Actor& actor, WeaponItemSubSystem::Projectiles_t& projectiles )
{
    std::auto_ptr<Actor> ps;
    static ActorFactory& mActorFactory = ActorFactory::Get();
    for ( int i = 0; i < explode.GetCount(); ++i )
    {
        ps = mActorFactory( explode.GetExplosionProjectile() );
        if ( ps->GetId() == -1 )
        {
            continue;
        }
        Opt<IOwnerComponent> actorOwnerC = actor.Get<IOwnerComponent>();
        Opt<IOwnerComponent> psOwnerC = ps->Get<IOwnerComponent>();
        if ( actorOwnerC.IsValid() && psOwnerC.IsValid() )
        {
            psOwnerC->SetOwnerGUID( actorOwnerC->GetOwnerGUID() );
        }
        if ( explode.GetCount() > 1 )
        {
            if ( explode.GetDistribution() == ExplodeDistributionType::Normal )
            {
                ps->Get<IPositionComponent>()->SetOrientation( -1 * explode.GetExplosionScatter() + i *
                        ( explode.GetExplosionScatter() / ( explode.GetCount() - 1 ) * 2 ) );
            }
            else if ( explode.GetDistribution() == ExplodeDistributionType::Random )
            {
                ps->Get<IPositionComponent>()->SetOrientation( -1 * explode.GetExplosionScatter()
                        + ( rand() % 100 ) * 0.01 * ( explode.GetExplosionScatter() * 2 ) );
            }
        }
        Opt<IPositionComponent> projPositionC( ps->Get<IPositionComponent>() );
        Opt<IPositionComponent> actorPositionC = actor.Get<IPositionComponent>();
        const double h = actorPositionC->GetOrientation() + projPositionC->GetOrientation();
        const double c = cos( h );
        const double s = sin( h );
        if ( explode.GetPositionVariance() != 0 )
        {
            const double shft = ( rand() % explode.GetPositionVariance() );
            projPositionC->SetX( c * shft );
            projPositionC->SetY( s * shft );
        }
        Opt<IMoveComponent> projMoveC( ps->Get<IMoveComponent>() );
        if ( projMoveC.IsValid() )
        {
            projMoveC->SetSpeed( projMoveC->GetSpeed().mBase.Get()
                                 * ( 1 - explode.GetSpeedVariance()
                                     + ( rand() % 100 ) * 0.01 * explode.GetSpeedVariance() * 2 ) );
        }
        Opt<IFadeOutComponent> fadeOutC( ps->Get<IFadeOutComponent>() );
        if ( fadeOutC.IsValid() )
        {
            fadeOutC->SetSecsToEnd( fadeOutC->GetSecsToEnd()
                                    * ( 1 - explode.GetSecsToEndVariance()
                                        + ( rand() % 100 ) * 0.01 * explode.GetSecsToEndVariance() * 2 ) );
        }
        projectiles.push_back( Opt<Actor>( ps.release() ) );
    }
}



} // namespace engine

