#include "text.h"


Text::Text( double fontSize, glm::vec4 dimensions, glm::vec4 color, std::string const& text, bool alignMiddle )
    : mFontSize( fontSize )
    , mDimensions( dimensions )
    , mColor( color )
    , mText( text )
    , mAlignMiddle( alignMiddle )
{

}

Text::Text()
    : mFontSize( 70 )
    , mDimensions( 0, 0, 500, 500 )
    , mColor( 1.0, 1.0, 1.0, 1.0 )
    , mText( "" )
    , mAlignMiddle( true )
{

}
