#ifndef INCLUDED_RENDER_CTF_HAT_RECOGNIZER_H
#define INCLUDED_RENDER_CTF_HAT_RECOGNIZER_H

#include "platform/i_platform.h"
#include "render/recognizer.h"

namespace render {
namespace ctf {

class CtfHatRecognizer : public Recognizer
{
public:
    CtfHatRecognizer( int32_t Id );
    virtual bool Recognize( Actor const& );
private:
};

} // namespace ctf
} // namespace render

#endif//INCLUDED_RENDER_CTF_HAT_RECOGNIZER_H

//command:  "classgenerator.exe" -g "recognizer" -c "ctf_hat_recognizer"
