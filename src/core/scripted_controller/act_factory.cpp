#include "platform/i_platform.h"
#include "act_factory.h"
#include "freelance_act.h"
#include "stay_act.h"

using platform::AutoId;
namespace scriptedcontroller {

ActFactory::ActFactory()
{
    Bind( AutoId("default_act"), &CreateIAct<DefaultAct> );
    SetDefault( AutoId("default_act") );

    Bind( AutoId( "freelance" ), &CreateIAct<FreelanceAct> );
    Bind( AutoId( "stay" ), &CreateIAct<StayAct> );
}

} // namespace scriptedcontroller

