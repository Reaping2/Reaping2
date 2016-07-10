#include "render/head_recognizer.h"

namespace render {

HeadRecognizer::HeadRecognizer( int32_t Id )
    : Recognizer( Id )
{
}


bool HeadRecognizer::Recognize( Actor const& actor ) const
{
    return true;
}

} // namespace render

