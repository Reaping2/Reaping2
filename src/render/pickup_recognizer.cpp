#include "render/pickup_recognizer.h"
#include "core/pickup_collision_component.h"
#include "core/i_health_component.h"
namespace render {

PickupRecognizer::PickupRecognizer( int32_t Id )
    : Recognizer( Id )
{

}

bool PickupRecognizer::Recognize( Actor const& actor ) const
{
    Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
    if ( healthC.IsValid() && !healthC->IsAlive() )
    {
        return false;
    }
    return Opt<PickupCollisionComponent>(actor.Get<ICollisionComponent>()).IsValid();
}

} // namespace render
