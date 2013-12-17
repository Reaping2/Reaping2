#ifndef INCLUDED_UI_WIDGET_FACTORY_H
#define INCLUDED_UI_WIDGET_FACTORY_H

class WidgetFactory : protected Factory<Widget>, public Singleton<WidgetFactory>
{
	friend class Singleton<WidgetFactory>;
	WidgetFactory();
public:
	std::auto_ptr<Widget> operator()(std::string const& Name, Json::Value& Initer);
};

#endif//INCLUDED_UI_WIDGET_FACTORY_H
