#include "text.h"


Text::Text(double fontSize, glm::vec4 dimensions, glm::vec4 color, std::string const& text, glm::vec2 position, bool alignMiddle)
    : mFontSize(fontSize)
    , mDimensions(dimensions)
    , mColor(color)
    , mText(text)
    , mPosition(position)
    , mAlignMiddle(alignMiddle)
{

}

Text::Text()
    : mFontSize(70)
    , mDimensions(0,0,500,500)
    , mColor(1.0,1.0,1.0,1.0)
    , mText("")
    , mPosition(0,0)
    , mAlignMiddle(true)
{

}
