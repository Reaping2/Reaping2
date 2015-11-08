#include "platform/i_platform.h"
#include "network/message_handler_sub_system_factory.h"
#include "platform/auto_id.h"
#include "my_name_message_handler_sub_system.h"
#include "client_id_message_handler_sub_system.h"
#include "lifecycle_message_handler_sub_system.h"
#include "create_actor_message_handler_sub_system.h"
#include "position_message_handler_sub_system.h"
#include "move_message_handler_sub_system.h"
#include "player_controller_message.h"
#include "damage_taken_message.h"
#include "orientation_message.h"
#include "heading_message.h"
#include "pickup_message.h"
#include "ping_message.h"
#include "revive_message.h"
#include "flash_message.h"
#include "heal_taken_message.h"
#include "soldier_properties_message.h"
#include "client_datas_message.h"
#include "health_message.h"
#include "accuracy_message.h"
#include "set_ownership_message.h"
#include "ctf_client_datas_message.h"
#include "set_team_message.h"
#include "ctf_score_message.h"
#include "show_text_message_message.h"
#include "collision_message.h"

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
    Bind( AutoId("heal_taken_message_handler_sub_system"), &CreateSubSystem<HealTakenMessageHandlerSubSystem>);
    Bind( AutoId("orientation_message_handler_sub_system"), &CreateSubSystem<OrientationMessageHandlerSubSystem>);
    Bind( AutoId("heading_message_handler_sub_system"), &CreateSubSystem<HeadingMessageHandlerSubSystem>);
    Bind( AutoId("pickup_message_handler_sub_system"), &CreateSubSystem<PickupMessageHandlerSubSystem>);
    Bind( AutoId("set_pickup_content_message_handler_sub_system"), &CreateSubSystem<SetPickupContentMessageHandlerSubSystem>);
    Bind( AutoId("ping_message_handler_sub_system"), &CreateSubSystem<PingMessageHandlerSubSystem>);
    Bind( AutoId("revive_message_handler_sub_system"), &CreateSubSystem<ReviveMessageHandlerSubSystem>);
    Bind( AutoId("flash_message_handler_sub_system"), &CreateSubSystem<FlashMessageHandlerSubSystem>);
    Bind( AutoId("soldier_properties_message_handler_sub_system"), &CreateSubSystem<SoldierPropertiesMessageHandlerSubSystem>);
    Bind( AutoId("client_datas_message_handler_sub_system"), &CreateSubSystem<ClientDatasMessageHandlerSubSystem>);
    Bind( AutoId("ctf_client_datas_message_handler_sub_system"), &CreateSubSystem<ctf::ClientDatasMessageHandlerSubSystem>);
    Bind( AutoId("health_message_handler_sub_system"), &CreateSubSystem<HealthMessageHandlerSubSystem>);
    Bind( AutoId("accuracy_message_handler_sub_system"), &CreateSubSystem<AccuracyMessageHandlerSubSystem>);
    Bind( AutoId("set_team_message_handler_sub_system"), &CreateSubSystem<SetTeamMessageHandlerSubSystem>);
    Bind( AutoId("ctf_score_message_handler_sub_system"), &CreateSubSystem<ctf::CtfScoreMessageHandlerSubSystem>);
    Bind( AutoId("show_text_message_message_handler_sub_system"), &CreateSubSystem<ShowTextMessageMessageHandlerSubSystem>);
    Bind( AutoId("collision_message_handler_sub_system"), &CreateSubSystem<CollisionMessageHandlerSubSystem>);

}

} // namespace network
