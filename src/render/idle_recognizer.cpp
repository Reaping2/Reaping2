#include "render/idle_recognizer.h"
#include "core/i_move_component.h"
namespace render {

IdleRecognizer::IdleRecognizer( int32_t Id )
    : Recognizer( Id )
{

}

bool IdleRecognizer::Recognize( Actor const& actor ) const
{
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    if ( !moveC.IsValid() )
    {
        return true;
    }
    if ( !moveC->IsMoving() )
    {
        //L1( " Idle RECOGNIZED! \n" );
        return true;
    }
    return false;
}

} // namespace render
