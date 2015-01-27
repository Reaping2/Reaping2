#ifndef INCLUDED_RENDER_IDLE_RECOGNIZER_H
#define INCLUDED_RENDER_IDLE_RECOGNIZER_H
#include "platform/i_platform.h"
#include "render/recognizer.h"
namespace render {

    class IdleRecognizer: public Recognizer
    {
    public:
        IdleRecognizer( int32_t Id );
        virtual bool Recognize(Actor const&);
    };

} // namespace render

#endif //INCLUDED_RENDER_IDLE_RECOGNIZER_H