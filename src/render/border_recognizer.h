#ifndef INCLUDED_RENDER_BORDER_RECOGNIZER_H
#define INCLUDED_RENDER_BORDER_RECOGNIZER_H

#include "platform/i_platform.h"
#include "render/recognizer.h"

namespace render {

class BorderRecognizer : public Recognizer
{
public:
    BorderRecognizer( int32_t Id );
    virtual bool Recognize( Actor const& ) const;
private:
};

} // namespace render

#endif//INCLUDED_RENDER_BORDER_RECOGNIZER_H

//command:  "classgenerator" -g "recognizer" -c "border_recognizer"
