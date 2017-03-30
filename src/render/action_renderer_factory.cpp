#include "platform/i_platform.h"
#include "render/action_renderer_factory.h"
#include "render/idle_action_renderer.h"
#include "render/move_action_renderer.h"
#include "render/leg_move_action_renderer.h"
#include "render/death_action_renderer.h"
#include "render/weapon_action_renderer.h"
#include "render/pickup_action_renderer.h"
#include "render/hat_action_renderer.h"
#include "ctf_hat_action_renderer.h"
#include "ctf_flag_action_renderer.h"
#include "soldier_current_action_renderer.h"
#include "border_action_renderer.h"
#include "head_color_action_renderer.h"
#include "head_action_renderer.h"
#include "ctf_head_color_action_renderer.h"
#include "cell_action_renderer.h"
#include "lit_action_renderer.h"
namespace render {
ActionRendererFactory::ActionRendererFactory()
{
    Bind<DefaultActionRenderer>( AutoId( "default_action_renderer" ) );
    SetDefault( AutoId( "default_action_renderer" ) );

    Bind<IdleActionRenderer>( AutoId( "idle" ) );
    Bind<MoveActionRenderer>( AutoId( "move" ) );
    Bind<LegMoveActionRenderer>( AutoId( "leg_move" ) );
    Bind<DeathActionRenderer>( AutoId( "death" ) );
    Bind<WeaponActionRenderer>( AutoId( "weapon" ) );
    Bind<PickupActionRenderer>( AutoId( "pickup" ) );
    Bind<HatActionRenderer>( AutoId( "body_color" ) );
    Bind<ctf::CtfHatActionRenderer>( AutoId( "ctf_body_color" ) );
    Bind<ctf::CtfFlagActionRenderer>( AutoId( "flag" ) );
    Bind<ctf::SoldierCurrentActionRenderer>( AutoId( "soldier_current" ) );
    Bind<BorderActionRenderer>( AutoId( "border" ) );
    Bind<HeadColorActionRenderer>( AutoId( "head_color" ) );
    Bind<HeadActionRenderer>( AutoId( "head" ) );
    Bind<CtfHeadColorActionRenderer>( AutoId( "ctf_head_color" ) );
    Bind<CellActionRenderer>( AutoId( "cell" ) );
    Bind<LitActionRenderer>( AutoId( "lit" ) );

}
} // namespace render
