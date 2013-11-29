#include "i_ui.h"

TextWidget::TextWidget(int32_t Id)
: Widget(Id)
{
}

void TextWidget::Init( Json::Value& Descriptor )
{
	BaseClass::Init(Descriptor);
	std::string s;
	int32_t i;
	if(Json::GetInt(Descriptor["color"],i))
		operator()(PT_Color)=i;
	else if(Json::GetStr(Descriptor["color"],s)&&(i=strtol(s.c_str(),NULL,16)))
		operator()(PT_Color)=i;
	else
		operator()(PT_Color)=0;
	operator()(PT_Text)=Json::GetStr(Descriptor["text"],s)?s:std::string();
}
