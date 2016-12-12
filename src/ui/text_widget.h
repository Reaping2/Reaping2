#ifndef INCLUDED_UI_TEXT_WIDGET_H
#define INCLUDED_UI_TEXT_WIDGET_H

#include "ui/widget.h"

class TextWidget : public Widget
{
    typedef Widget BaseClass;
public:
    enum class VAlign {
        Top,
        Mid,
        Bottom,
    };
    enum class HAlign {
        Left,
        Center,
        Right,
    };
    TextWidget( int32_t Id );
    virtual void Init( Json::Value& Descriptor );
};

#endif//INCLUDED_UI_TEXT_WIDGET_H
