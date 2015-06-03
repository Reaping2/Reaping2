#include "platform/i_platform.h"
#include "engine/controllers/random_controller_sub_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "core/random_controller_component.h"
#include "core/i_move_component.h"
#include "core/i_position_component.h"
#include "core/i_health_component.h"

namespace engine {

RandomControllerSubSystem::RandomControllerSubSystem()
    : mScene( Scene::Get() )
    , mProgramState (core::ProgramState::Get())
{

}

void RandomControllerSubSystem::Init()
{


}

void RandomControllerSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<RandomControllerComponent> randomCC = actor.Get<RandomControllerComponent>();
    if (!randomCC.IsValid()||!randomCC->IsEnabled())
    {
        return;
    }
    Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
    if (healthC.IsValid()&&!healthC->IsAlive())
    {
        return;
    }
    if (mProgramState.mMode!=core::ProgramState::Client)
    {
        randomCC->SetCounter(randomCC->GetCounter()+DeltaTime);
        if ( randomCC->GetCounter() > 2 )
        {
            randomCC->SetCounter(0);
            randomCC->SetHeadingModifier(( rand() % 10 - 5 ) * .33);

            Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
            if (!moveC.IsValid())
            {
                return;
            }
            moveC->GetSpeed().mBase.Set(( rand() % 10 )*30);
            moveC->SetMoving(moveC->GetSpeed().Get()!=0);

            moveC->SetHeadingModifier( randomCC->GetHeadingModifier() );
        }
    }
    actor.Get<IPositionComponent>()->SetOrientation( actor.Get<IMoveComponent>()->GetHeading() );
}

} // namespace engine

