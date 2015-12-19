#include "i_ui.h"

WidgetFactory::WidgetFactory()
{
    Bind<Widget>( AutoId( "widget" ) );
    SetDefault( AutoId( "widget" ) );
    Bind<FixedRatioContainer>( AutoId( "fixed_ratio_container" ) );
    Bind<TextWidget>( AutoId( "text_widget" ) );
    Bind<Button>( AutoId( "button" ) );
    Bind<Bar>( AutoId( "bar" ) );
    Bind<ImageWidget>( AutoId( "image" ) );
    Bind<GridWidget>( AutoId( "grid" ) );
}

std::auto_ptr<Widget> WidgetFactory::operator()( std::string const& Name, Json::Value& Initer )
{
    std::auto_ptr<Widget> Wdg = Factory<Widget>::operator()( AutoId( Name ) );
    assert( Wdg.get() );
    if( Wdg.get() )
    {
        Wdg->Init( Initer );
    }
    return Wdg;
}
