#include "i_core.h"

ComponentRepo::ComponentRepo()
{
    Bind( DefaultComponent::GetType_static(), &CreateComponent<DefaultComponent> );
    SetDefault( DefaultComponent::GetType_static() );

    Bind( PositionComponent::GetType_static(), &CreateComponent<PositionComponent>);

}
