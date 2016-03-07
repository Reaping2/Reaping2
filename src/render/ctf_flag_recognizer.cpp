#include "render/ctf_flag_recognizer.h"

namespace render {
namespace ctf {

CtfFlagRecognizer::CtfFlagRecognizer( int32_t Id )
    : Recognizer( Id )
{
}


bool CtfFlagRecognizer::Recognize( Actor const& actor )
{
    return true;
}

} // namespace ctf
} // namespace render

