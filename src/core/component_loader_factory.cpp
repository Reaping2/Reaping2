#include "i_core.h"
#include "core/component_loader_factory.h"
#include "core/health_component.h"


ComponentLoaderFactory::ComponentLoaderFactory()
{
    Bind( AutoId( "default_component" ), &CreateComponentLoader<DefaultComponentLoader>);
    SetDefault( AutoId( "default_component" ) );
    Bind( AutoId( "health_component" ), &CreateComponentLoader<HealthComponentLoader>);
}

void DefaultComponentLoader::BindValues()
{

}
DefaultComponentLoader::DefaultComponentLoader()
{
}



