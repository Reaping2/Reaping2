#ifndef INCLUDED_RENDER_HEAD_RECOGNIZER_H
#define INCLUDED_RENDER_HEAD_RECOGNIZER_H

#include "platform/i_platform.h"
#include "render/recognizer.h"

namespace render {

class HeadRecognizer : public Recognizer
{
public:
    HeadRecognizer( int32_t Id );
    virtual bool Recognize(Actor const&);
private:
};

} // namespace render

#endif//INCLUDED_RENDER_HEAD_RECOGNIZER_H

//command:  "classgenerator.exe" -g "recognizer" -c "head_recognizer"
