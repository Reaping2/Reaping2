#include "render/leg_move_recognizer.h"
#include "core/i_move_component.h"
namespace render {

LegMoveRecognizer::LegMoveRecognizer( int32_t Id )
    : Recognizer( Id )
{

}

bool LegMoveRecognizer::Recognize( Actor const& actor )
{
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    if ( !moveC.IsValid() )
    {
        return false;
    }
    if ( moveC->IsMoving() )
    {
        //L1( " leg move RECOGNIZED! \n" );
        return true;
    }
    return false;
}

} // namespace render