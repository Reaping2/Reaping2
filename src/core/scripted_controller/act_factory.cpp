#include "platform/i_platform.h"
#include "act_factory.h"
#include "freelance_act.h"
#include "stay_act.h"
#include "bite_act.h"
#include "do_nothing_act.h"
#include "move_forward_act.h"
#include "strafe_act.h"
#include "select_target_act.h"
#include "turn_to_target_act.h"
#include "turn_away_act.h"

using platform::AutoId;
namespace scriptedcontroller {

ActFactory::ActFactory()
{
    Bind( AutoId("default_act"), &CreateIAct<DefaultAct> );
    SetDefault( AutoId("default_act") );

    Bind( AutoId( "freelance" ), &CreateIAct<FreelanceAct> );
    Bind( AutoId( "stay" ), &CreateIAct<StayAct> );
    Bind( AutoId( "bite" ), &CreateIAct<BiteAct> );
    Bind( AutoId( "do_nothing" ), &CreateIAct<DoNothingAct> );
    Bind( AutoId( "move_forward" ), &CreateIAct<MoveForwardAct> );
    Bind( AutoId( "strafe" ), &CreateIAct<StrafeAct> );
    Bind( AutoId( "select_target" ), &CreateIAct<SelectTargetAct> );
    Bind( AutoId( "turn_to_target" ), &CreateIAct<TurnToTargetAct> );
    Bind( AutoId( "turn_away" ), &CreateIAct<TurnAwayAct> );
}

} // namespace scriptedcontroller

