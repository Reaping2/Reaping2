#ifndef INCLUDED_CTF_I_FLAG_RECEIVER_COMPONENT_H
#define INCLUDED_CTF_I_FLAG_RECEIVER_COMPONENT_H

#include "component.h"
#include "platform/export.h"

namespace ctf {

class IFlagReceiverComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IFlagReceiverComponent)
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void IFlagReceiverComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}

} // namespace ctf


REAPING2_CLASS_EXPORT_KEY2(ctf__IFlagReceiverComponent, ctf::IFlagReceiverComponent,"i_flag_receiver_component");
#endif//INCLUDED_CTF_I_FLAG_RECEIVER_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_flag_receiver_component" -n "ctf"
