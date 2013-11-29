#ifndef INCLUDED_UI_TEXT_WIDGET_H
#define INCLUDED_UI_TEXT_WIDGET_H

class TextWidget : public Widget
{
	typedef Widget BaseClass;
public:
	TextWidget(int32_t Id);
	virtual void Init(Json::Value& Descriptor);
};

#endif//INCLUDED_UI_TEXT_WIDGET_H
