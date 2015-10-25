#ifndef INCLUDED_CTF_I_FLAG_RECEIVER_COMPONENT_H
#define INCLUDED_CTF_I_FLAG_RECEIVER_COMPONENT_H

#include "component.h"

namespace ctf {

class IFlagReceiverComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IFlagReceiverComponent)
};

} // namespace ctf

#endif//INCLUDED_CTF_I_FLAG_RECEIVER_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_flag_receiver_component" -n "ctf"
