#ifndef INCLUDED_RENDER_DEATH_RECOGNIZER_H
#define INCLUDED_RENDER_DEATH_RECOGNIZER_H
#include "platform/i_platform.h"
#include "render/recognizer.h"
namespace render {

class DeathRecognizer: public Recognizer
{
public:
    DeathRecognizer( int32_t Id );
    virtual bool Recognize( Actor const& ) const;
};

} // namespace render

#endif //INCLUDED_RENDER_DEATH_RECOGNIZER_H