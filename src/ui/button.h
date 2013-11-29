#ifndef INCLUDED_UI_BUTTON_H
#define INCLUDED_UI_BUTTON_H

class Button : public Widget
{
	typedef Widget BaseClass;
	struct ActionDesc
	{
		std::string mAction;
		Widget::Prop mArg;
	};
	typedef std::vector<ActionDesc> Actions_t;
	Actions_t mActions;
	void AddAction( Json::Value& TriggerAction );
public:
	Button(int32_t Id);
	virtual void Init(Json::Value& Descriptor);
	virtual bool Trigger();
};

#endif//INCLUDED_UI_BUTTON_H
