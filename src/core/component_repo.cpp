#include "i_core.h"

ComponentRepo::ComponentRepo()
{
    Bind<DefaultComponent>( AutoId( "default_component" ) );
    SetDefault( AutoId( "default_component" ) );

    Bind<PositionComponent>( AutoId( "position_component" ) );
}
