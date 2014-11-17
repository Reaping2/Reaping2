#include "i_core.h"
#include "move_component.h"
#include "platform/auto_id.h"

using platform::AutoId;

ComponentFactory::ComponentFactory()
{
    Bind( AutoId("default_component"), &CreateComponent<DefaultComponent> );
    SetDefault( AutoId("default_component") );

    Bind( AutoId("position_component"), &CreateComponent<PositionComponent>);
    Bind( AutoId("move_component"), &CreateComponent<MoveComponent>);
}
