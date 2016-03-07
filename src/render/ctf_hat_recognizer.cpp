#include "render/ctf_hat_recognizer.h"
#include "core/player_controller_component.h"
#include "core/i_health_component.h"

namespace render {
namespace ctf {

CtfHatRecognizer::CtfHatRecognizer( int32_t Id )
    : Recognizer( Id )
{
}


bool CtfHatRecognizer::Recognize( Actor const& actor )
{
    //     Opt<PlayerControllerComponent> playerCC=actor.Get<PlayerControllerComponent>();
    //     if (!playerCC.IsValid())
    //     {
    //         return false;
    //     }
    //     Opt<IHealthComponent> healthC=actor.Get<IHealthComponent>();
    //     if (!healthC.IsValid()||!healthC->IsAlive())
    //     {
    //         return false;
    //     }
    return true;
}

} // namespace ctf
} // namespace render

