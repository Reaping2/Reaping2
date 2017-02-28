#include "platform/i_platform.h"
#include "kill_score_on_death_system.h"
#include "core/i_kill_score_on_death_component.h"
#include "core/i_health_component.h"
#include "kill_score_event.h"

namespace engine {

KillScoreOnDeathSystem::KillScoreOnDeathSystem()
    : mScene( Scene::Get() )
{
}


void KillScoreOnDeathSystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IKillScoreOnDeathComponent>().IsValid()
            && actor.Get<IHealthComponent>().IsValid(); } );
}


void KillScoreOnDeathSystem::Update( double DeltaTime )
{
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
        Opt<IKillScoreOnDeathComponent> killScoreOnDeathC = actor->Get<IKillScoreOnDeathComponent>();
        if ( !killScoreOnDeathC.IsValid() )
        {
            continue;
        }
        Opt<IHealthComponent> healthC = actor->Get<IHealthComponent>();
        if ( !killScoreOnDeathC->IsScored() && healthC.IsValid() && !healthC->IsAlive() )
        {
            L2( "KillScoreOnDeath sent" );
            EventServer<KillScoreEvent>::Get().SendEvent( KillScoreEvent( healthC->GetLastDamageOwnerGUID(), actor->GetGUID() ) );
            killScoreOnDeathC->SetScored( true );
        }
    }
}


} // namespace engine

