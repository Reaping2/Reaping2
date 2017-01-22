#include "render/move_recognizer.h"
#include "core/i_move_component.h"
namespace render {

MoveRecognizer::MoveRecognizer( int32_t Id )
    : Recognizer( Id )
{

}

bool MoveRecognizer::Recognize( Actor const& actor ) const
{
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    if ( !moveC.IsValid() )
    {
        return false;
    }
    if ( moveC->IsMoving() )
    {
        //L1( " body move RECOGNIZED! \n" );
        return true;
    }
    return false;
}

} // namespace render
