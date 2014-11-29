#include "platform/i_platform.h"
#include "core/action_repo.h"
#include "core/damage_action.h"
#include "move_action.h"
#include "idle_action.h"
#include "fade_out_action.h"
#include "shoot_action.h"
#include "death_action.h"
#include "core/action.h"

ActionRepo::ActionRepo()
{
    Bind<DefaultAction>( AutoId( "default_action" ) );
    SetDefault( AutoId( "default_action" ) );

    Bind<MoveAction>( AutoId( "move" ) );
    Bind<ShootAction>( AutoId( "shoot" ) );
    Bind<ShootAltAction>( AutoId( "shoot_alt" ) );
    Bind<IdleAction>( AutoId( "idle" ) );
    Bind<DeathAction>( AutoId( "death" ) );
    Bind<FadeOutAction>( AutoId( "fade_out" ) );
    Bind<DamageAction>( AutoId( "damage" ) );
}
