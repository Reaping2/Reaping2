#include "i_core.h"
#include "move_component.h"

ComponentRepo::ComponentRepo()
{
    Bind( DefaultComponent::GetType(), &CreateComponent<DefaultComponent> );
    SetDefault( DefaultComponent::GetType() );

    Bind( PositionComponent::GetType(), &CreateComponent<PositionComponent>);
    Bind( MoveComponent::GetType(), &CreateComponent<MoveComponent>);
}
