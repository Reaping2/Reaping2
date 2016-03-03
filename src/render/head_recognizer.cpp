#include "render/head_recognizer.h"

namespace render {

HeadRecognizer::HeadRecognizer(int32_t Id)
    : Recognizer(Id)
{
}


bool HeadRecognizer::Recognize(Actor const& actor)
{
    return true;
}

} // namespace render

