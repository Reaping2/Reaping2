#include "platform/i_platform.h"
#include "act_factory.h"

using platform::AutoId;
namespace scriptedcontroller {

ActFactory::ActFactory()
{
    Bind( AutoId("default_act"), &CreateIAct<DefaultAct> );
    SetDefault( AutoId("default_act") );
}

} // namespace scriptedcontroller

