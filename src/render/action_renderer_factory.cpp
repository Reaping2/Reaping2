#include "platform/i_platform.h"
#include "render/action_renderer_factory.h"
#include "render/idle_action_renderer.h"
#include "render/body_move_action_renderer.h"
#include "render/leg_move_action_renderer.h"
#include "render/death_action_renderer.h"
#include "render/weapon_action_renderer.h"
#include "render/pickup_action_renderer.h"
#include "render/hat_action_renderer.h"
#include "ctf_hat_action_renderer.h"
#include "ctf_flag_action_renderer.h"
namespace render {
    ActionRendererFactory::ActionRendererFactory()
    {
        Bind<DefaultActionRenderer>( AutoId( "default_action_renderer" ) );
        SetDefault( AutoId( "default_action_renderer" ) );

        Bind<IdleActionRenderer>( AutoId( "idle" ) );
        Bind<BodyMoveActionRenderer>( AutoId( "body_move" ) );
        Bind<LegMoveActionRenderer>( AutoId( "leg_move" ) );
        Bind<LegMoveActionRenderer>( AutoId( "move" ) );
        Bind<DeathActionRenderer>( AutoId( "death" ) );
        Bind<WeaponActionRenderer>( AutoId( "weapon" ) );
        Bind<PickupActionRenderer>( AutoId( "pickup" ) );
        Bind<HatActionRenderer>( AutoId( "hat" ) );
        Bind<ctf::CtfHatActionRenderer>(AutoId("ctf_hat"));
        Bind<ctf::CtfFlagActionRenderer>(AutoId("flag"));
    }
} // namespace render
