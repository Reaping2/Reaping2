#include "render/death_recognizer.h"
#include "core/i_health_component.h"
namespace render {

    DeathRecognizer::DeathRecognizer(int32_t Id)
        : Recognizer(Id)
    {

    }

    bool DeathRecognizer::Recognize(Actor const& actor)
    {
        Opt<IHealthComponent> healthC=actor.Get<IHealthComponent>();
        if (!healthC.IsValid())
        {
            return false;
        }
        if (!healthC->IsAlive())
        {
            //L1( " death RECOGNIZED! (lol) (hahaha) (sofunny) ... ((U DEAD!)) (lululuuuu) \n" );
            return true;
        }
        return false;
    }

} // namespace render