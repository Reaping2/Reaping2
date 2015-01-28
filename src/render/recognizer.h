#ifndef INCLUDED_RENDER_RECOGNIZER_H
#define INCLUDED_RENDER_RECOGNIZER_H
#include "platform/i_platform.h"
#include "core/actor.h"

namespace render {

class Recognizer
{
public:
    virtual ~Recognizer();
    Recognizer( int32_t Id );
    virtual bool Recognize(Actor const& actor);
    
    void SetActionRenderer(int32_t actionRendererId);
    int32_t GetActionRenderer() const;
protected:
    int32_t mId;
    int32_t mActionRenderer;
    int32_t mOrder;
public:
    int32_t GetId() const
    {
        return mId;
    }
    int32_t GetOrder() const
    {
        return mOrder;
    }
    void SetOrder(int32_t order)
    {
        mOrder=order;
    }
};

class DefaultRecognizer : public Recognizer
{
public:
    DefaultRecognizer( int32_t Id );
};

} // namespace render

#endif //INCLUDED_RENDER_RECOGNIZER_H
