#include "render/body_move_recognizer.h"
#include "core/i_move_component.h"
namespace render {

    BodyMoveRecognizer::BodyMoveRecognizer(int32_t Id)
        : Recognizer(Id)
    {

    }

    bool BodyMoveRecognizer::Recognize(Actor const& actor)
    {
        Opt<IMoveComponent> moveC=actor.Get<IMoveComponent>();
        if (!moveC.IsValid())
        {
            return false;
        }
        if (moveC->IsMoving())
        {
            //L1( " body move RECOGNIZED! \n" );
            return true;
        }
        return false;
    }

} // namespace render