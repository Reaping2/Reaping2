#include "i_core.h"

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
}
