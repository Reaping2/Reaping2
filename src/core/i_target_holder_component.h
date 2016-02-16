#ifndef INCLUDED_CORE_I_TARGET_HOLDER_COMPONENT_H
#define INCLUDED_CORE_I_TARGET_HOLDER_COMPONENT_H

#include "component.h"

class ITargetHolderComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(ITargetHolderComponent)
    virtual void SetTargetGUID(int32_t targetId)=0;
    virtual int32_t GetTargetGUID()const=0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void ITargetHolderComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_TARGET_HOLDER_COMPONENT_H
