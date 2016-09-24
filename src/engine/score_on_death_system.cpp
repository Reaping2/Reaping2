#include "platform/i_platform.h"
#include "score_on_death_system.h"
#include "core/i_score_on_death_component.h"
#include "core/i_health_component.h"
#include "score_event.h"

namespace engine {

ScoreOnDeathSystem::ScoreOnDeathSystem()
    : mScene( Scene::Get() )
{
}


void ScoreOnDeathSystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IScoreOnDeathComponent>().IsValid()
            && actor.Get<IHealthComponent>().IsValid(); } );
}


void ScoreOnDeathSystem::Update( double DeltaTime )
{
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
        Opt<IScoreOnDeathComponent> scoreOnDeathC = actor->Get<IScoreOnDeathComponent>();
        if ( !scoreOnDeathC.IsValid() )
        {
            continue;
        }
        if( !scoreOnDeathC->IsScored() && !actor->Get<IHealthComponent>()->IsAlive() )
        {
            scoreOnDeathC->SetScored( true );
            EventServer<ScoreEvent>::Get().SendEvent( ScoreEvent( actor->GetGUID() ) );
        }
    }
}


} // namespace engine

