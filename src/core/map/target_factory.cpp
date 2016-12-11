#include "platform/i_platform.h"
#include "target_factory.h"

using platform::AutoId;
namespace map {

TargetFactory::TargetFactory()
{
    //Bind( AutoId("default_i_target"), &CreateITarget<DefaultITarget> );
    //SetDefault( AutoId("default_i_target") );
}

} // namespace map

