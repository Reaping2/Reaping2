#ifndef INCLUDED_CTF_I_FLAG_CARRIER_COMPONENT_H
#define INCLUDED_CTF_I_FLAG_CARRIER_COMPONENT_H

#include "component.h"

namespace ctf {

class IFlagCarrierComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IFlagCarrierComponent)
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void IFlagCarrierComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}

} // namespace ctf

#endif//INCLUDED_CTF_I_FLAG_CARRIER_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_flag_carrier_component" -n "ctf"
