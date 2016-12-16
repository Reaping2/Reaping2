#include "i_ui.h"
#include <boost/algorithm/string.hpp>

TextWidget::TextWidget( int32_t Id )
    : Widget( Id )
{
}

void TextWidget::Init( Json::Value& Descriptor )
{
    BaseClass::Init( Descriptor );
    ParseStrProp( PT_Text, Descriptor["text"], "" );
    ParseDoubleProp( PT_FontSize, Descriptor["fontsize"], 8.0 );
    std::string str;
    Prop& va = operator()( PT_VAlign );
    va = (int)VAlign::Top;
    if( Json::GetStr( Descriptor["valign"], str ) )
    {
        if( boost::iequals( str, "mid" ) )
        {
            va = (int)VAlign::Mid;
        }
        else if( boost::iequals( str, "bottom" ) )
        {
            va = (int)VAlign::Bottom;
        }
    }
    Prop& ha = operator()( PT_HAlign );
    ha = (int)HAlign::Left;
    if( Json::GetStr( Descriptor["halign"], str ) )
    {
        if( boost::iequals( str, "center" ) )
        {
            ha = (int)HAlign::Center;
        }
        else if( boost::iequals( str, "right" ) )
        {
            ha = (int)HAlign::Right;
        }
    }
}
