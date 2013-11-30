#include "i_ui.h"

TextWidget::TextWidget(int32_t Id)
: Widget(Id)
{
}

void TextWidget::Init( Json::Value& Descriptor )
{
	BaseClass::Init(Descriptor);
	std::string s;
	operator()(PT_Text)=Json::GetStr(Descriptor["text"],s)?s:std::string();
	double d;
	operator()(PT_FontSize)=Json::GetDouble(Descriptor["fontsize"],d)?d:8.0;
}
