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
}


void KillScoreOnDeathSystem::Update( double DeltaTime )
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
        Actor& actor = **it;
        Opt<IKillScoreOnDeathComponent> killScoreOnDeathC = actor.Get<IKillScoreOnDeathComponent>();
        if ( !killScoreOnDeathC.IsValid() )
        {
            continue;
        }
        Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
        if ( !killScoreOnDeathC->IsScored() && healthC.IsValid() && !healthC->IsAlive() )
        {
            L1( "KillScoreOnDeath sent" );
            EventServer<KillScoreEvent>::Get().SendEvent( KillScoreEvent( healthC->GetLastDamageOwnerGUID(), actor.GetGUID() ) );
            killScoreOnDeathC->SetScored( true );
        }
    }
}


} // namespace engine

