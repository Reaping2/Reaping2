#include "platform/i_platform.h"
#include "engine/system_factory.h"
#include "engine/system.h"
#include "engine/collision_system.h"
#include "platform/auto_id.h"
#include "input/keyboard.h"
#include "engine/controllers/player_controller_sub_system.h"
#include "engine/controllers/random_controller_sub_system.h"
#include "engine/controllers/target_player_controller_sub_system.h"
#include "engine/controllers/controller_system.h"
#include "engine/items/inventory_system.h"
#include "engine/drop_on_death_system.h"
#include "engine/health_system.h"

using platform::AutoId;
namespace engine {

SystemFactory::SystemFactory()
{
    Bind( AutoId("default_system"), &CreateSystem<DefaultSystem> );
    SetDefault( AutoId("default_system") );

    Bind( AutoId("collision_system"), &CreateSystem<CollisionSystem>);
    Bind( AutoId("keyboard_system"), &CreateSystem<KeyboardSystem>);
    Bind( AutoId("controller_system"), &CreateSystem<ControllerSystem>);
    Bind( AutoId("inventory_system"), &CreateSystem<InventorySystem>);
    Bind( AutoId("drop_on_death_system"), &CreateSystem<DropOnDeathSystem>);
    Bind( AutoId("health_system"), &CreateSystem<HealthSystem>);


}

} // namespace engine
