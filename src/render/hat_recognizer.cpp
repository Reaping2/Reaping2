#include "render/hat_recognizer.h"
#include "core/player_controller_component.h"
#include "core/i_health_component.h"
namespace render {

HatRecognizer::HatRecognizer( int32_t Id )
    : Recognizer( Id )
{

}

bool HatRecognizer::Recognize( Actor const& actor ) const
{
    Opt<PlayerControllerComponent> playerCC = actor.Get<PlayerControllerComponent>();
    if ( !playerCC.IsValid() )
    {
        return false;
    }
    Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
    if ( !healthC.IsValid() || !healthC->IsAlive() )
    {
        return false;
    }
    return true;
}

} // namespace render