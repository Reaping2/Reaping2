#include "platform/i_platform.h"
#include "network/message_handler_sub_system_factory.h"
#include "platform/auto_id.h"
#include "my_name_message_handler_sub_system.h"
#include "client_id_message_handler_sub_system.h"
#include "lifecycle_message_handler_sub_system.h"
#include "create_actor_message_handler_sub_system.h"
#include "set_ownership_message_handler_sub_system.h"
#include "position_message_handler_sub_system.h"
#include "move_message_handler_sub_system.h"
#include "player_controller_message.h"
#include "damage_taken_message.h"
#include "orientation_message.h"
#include "heading_message.h"

using platform::AutoId;
namespace network {

MessageHandlerSubSystemFactory::MessageHandlerSubSystemFactory()
{
    Bind( AutoId("default_message_handler_sub_system"), &CreateSubSystem<DefaultMessageHandlerSubSystem> );
    SetDefault( AutoId("default_message_handler_sub_system") );

    Bind( AutoId("my_name_message_handler_sub_system"), &CreateSubSystem<MyNameMessageHandlerSubSystem> );
    Bind( AutoId("client_id_message_handler_sub_system"), &CreateSubSystem<ClientIdMessageHandlerSubSystem> );
    Bind( AutoId("lifecycle_message_handler_sub_system"), &CreateSubSystem<LifecycleMessageHandlerSubSystem> );
    Bind( AutoId("create_actor_message_handler_sub_system"), &CreateSubSystem<CreateActorMessageHandlerSubSystem> );
    Bind( AutoId("set_ownership_message_handler_sub_system"), &CreateSubSystem<SetOwnershipMessageHandlerSubSystem> );
    Bind( AutoId("position_message_handler_sub_system"), &CreateSubSystem<PositionMessageHandlerSubSystem> );
    Bind( AutoId("move_message_handler_sub_system"), &CreateSubSystem<MoveMessageHandlerSubSystem>);
    Bind( AutoId("player_controller_message_handler_sub_system"), &CreateSubSystem<PlayerControllerMessageHandlerSubSystem>);
    Bind( AutoId("damage_taken_message_handler_sub_system"), &CreateSubSystem<DamageTakenMessageHandlerSubSystem>);
    Bind( AutoId("orientation_message_handler_sub_system"), &CreateSubSystem<OrientationMessageHandlerSubSystem>);
    Bind( AutoId("heading_message_handler_sub_system"), &CreateSubSystem<HeadingMessageHandlerSubSystem>);
}

} // namespace engine
