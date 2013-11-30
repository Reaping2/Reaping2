#include "i_ui.h"

TextWidget::TextWidget(int32_t Id)
: Widget(Id)
{
}

void TextWidget::Init( Json::Value& Descriptor )
{
	BaseClass::Init(Descriptor);
	ParseStrProp(PT_Text,Descriptor["text"],"");
	ParseDoubleProp(PT_FontSize,Descriptor["fontsize"],8.0);
}
