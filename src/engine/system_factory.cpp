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
#include "engine/remove_on_death_system.h"
#include "engine/fade_out_system.h"
#include "engine/move_system.h"
#include "input/mouse.h"
#include "main/window.h"
#include "platform/timer.h"
#include "engine/frame_counter_system.h"
#include "render/renderer.h"
#include "network/server_system.h"
#include "network/client_system.h"
#include "network/message_handler_sub_system_holder.h"

using platform::AutoId;
namespace engine {

SystemFactory::SystemFactory()
{
    Bind( AutoId("default_system"), &CreateSystem<DefaultSystem> );
    SetDefault( AutoId("default_system") );

    Bind( AutoId("window_system"), &CreateSystem<WindowSystem>);
    Bind( AutoId("timer_server_system"), &CreateSystem<TimerServerSystem>);
    Bind( AutoId("frame_counter_system"), &CreateSystem<FrameCounterSystem>);
    Bind( AutoId("renderer_system"), &CreateSystem<RendererSystem>);

    Bind( AutoId("collision_system"), &CreateSystem<CollisionSystem>);
    Bind( AutoId("keyboard_system"), &CreateSystem<KeyboardSystem>);
    Bind( AutoId("mouse_system"), &CreateSystem<MouseSystem>);
    Bind( AutoId("controller_system"), &CreateSystem<ControllerSystem>);
    Bind( AutoId("inventory_system"), &CreateSystem<InventorySystem>);
    Bind( AutoId("drop_on_death_system"), &CreateSystem<DropOnDeathSystem>);
    Bind( AutoId("health_system"), &CreateSystem<HealthSystem>);
    Bind( AutoId("remove_on_death_system"), &CreateSystem<RemoveOnDeathSystem>);
    Bind( AutoId("fade_out_system"), &CreateSystem<FadeOutSystem>);
    Bind( AutoId("move_system"), &CreateSystem<MoveSystem>);

    Bind( AutoId("server_system"), &CreateSystem<network::ServerSystem>);
    Bind( AutoId("client_system"), &CreateSystem<network::ClientSystem>);
    Bind( AutoId("message_handler_sub_system_holder"), &CreateSystem<network::MessageHandlerSubSystemHolder>);

}

} // namespace engine
