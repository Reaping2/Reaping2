#include "render/recognizer.h"

namespace render {
Recognizer::~Recognizer()
{
}

Recognizer::Recognizer( int32_t Id )
    : mId( Id )
    , mActionRenderer( 0 )
    , mOrder( 0 )
{

}
bool Recognizer::Recognize( Actor const& actor )
{
    return false;
}
int32_t Recognizer::GetActionRenderer() const
{
    return mActionRenderer;
}

void Recognizer::SetActionRenderer( int32_t actionRendererId )
{
    mActionRenderer = actionRendererId;
}


DefaultRecognizer::DefaultRecognizer( int32_t Id )
    : Recognizer( Id )
{

}


} // namespace render