#include "platform/i_platform.h"
#include "core/action_repo.h"
#include "move_action.h"
#include "idle_action.h"
#include "fade_out_action.h"
#include "core/action.h"

ActionRepo::ActionRepo()
{
    Bind<DefaultAction>( AutoId( "default_action" ) );
    SetDefault( AutoId( "default_action" ) );

    Bind<MoveAction>( AutoId( "move" ) );
    Bind<IdleAction>( AutoId( "idle" ) );
    Bind<FadeOutAction>( AutoId( "fade_out" ) );
}
