#include "platform/i_platform.h"
#include "engine/acceleration_system.h"
#include "core/i_move_component.h"
#include "core/i_acceleration_component.h"

namespace engine {

AccelerationSystem::AccelerationSystem()
    : mScene( Scene::Get() )
{

}

void AccelerationSystem::Init()
{
}

void AccelerationSystem::Update(double DeltaTime)
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
        Actor& actor = **it;
        Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
        if (!moveC.IsValid())
        {
            continue;
        }
        Opt<IAccelerationComponent> accelerationC = actor.Get<IAccelerationComponent>();
        if (!accelerationC.IsValid())
        {
            continue;
        }
        
        double speed=moveC->GetSpeed().mBase.Get();
        double acceleration=accelerationC->GetAcceleration();
        speed+=acceleration*DeltaTime;
        speed=(acceleration>0.0)?
            std::min(speed,accelerationC->GetMaxSpeed()):
            std::max(speed,accelerationC->GetMinSpeed());
        moveC->GetSpeed().mBase.Set(speed);
    }

}

} // namespace engine

