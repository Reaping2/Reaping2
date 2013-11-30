#include "i_ui.h"

WidgetFactory::WidgetFactory()
{
	Bind<Widget>(AutoId("widget"));
	SetDefault(AutoId("widget"));
	Bind<FixedRatioContainer>(AutoId("fixed_ratio_container"));
	Bind<TextWidget>(AutoId("text_widget"));
	Bind<Button>(AutoId("button"));
	Bind<Bar>(AutoId("bar"));
}

Widget* WidgetFactory::operator()( std::string const& Name, Json::Value& Initer )
{
	Widget* Wdg=Factory<Widget>::operator()(AutoId(Name));
	assert(Wdg);
	if(Wdg)
		Wdg->Init(Initer);
	return Wdg;
}
