#ifndef INCLUDED_CTF_I_FLAG_CARRIER_COMPONENT_H
#define INCLUDED_CTF_I_FLAG_CARRIER_COMPONENT_H

#include "component.h"

namespace ctf {

class IFlagCarrierComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IFlagCarrierComponent)
};

} // namespace ctf

#endif//INCLUDED_CTF_I_FLAG_CARRIER_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_flag_carrier_component" -n "ctf"
