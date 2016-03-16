#ifndef INCLUDED_RENDER_TEXT_H
#define INCLUDED_RENDER_TEXT_H
#include "platform/i_platform.h"
struct Text
{
    double mFontSize;    //50 is a small yet visible font
    glm::vec4 mDimensions;  //posx, posy, width, height
    glm::vec4 mColor;
    std::string mText;
    bool mAlignMiddle;
    Text();
    Text( double fontSize, glm::vec4 dimensions, glm::vec4 color, std::string const& text, bool alignMiddle );
};

#endif//INCLUDED_RENDER_TEXT_H
