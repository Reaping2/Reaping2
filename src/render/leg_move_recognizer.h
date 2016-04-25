#ifndef INCLUDED_RENDER_LEG_MOVE_RECOGNIZER_H
#define INCLUDED_RENDER_LEG_MOVE_RECOGNIZER_H
#include "platform/i_platform.h"
#include "render/recognizer.h"
namespace render {

class LegMoveRecognizer: public Recognizer
{
public:
    LegMoveRecognizer( int32_t Id );
    virtual bool Recognize( Actor const& ) const;
};

} // namespace render

#endif //INCLUDED_RENDER_LEG_MOVE_RECOGNIZER_H
