#include "render/border_recognizer.h"
#include "core/i_border_component.h"

namespace render {

BorderRecognizer::BorderRecognizer( int32_t Id )
    : Recognizer( Id )
{
}


bool BorderRecognizer::Recognize( Actor const& actor )
{
    Opt<IBorderComponent> borderC = actor.Get<IBorderComponent>();
    if ( !borderC.IsValid() )
    {
        return false;
    }
    return true;
}

} // namespace render

