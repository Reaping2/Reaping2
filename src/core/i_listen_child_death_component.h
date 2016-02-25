#ifndef INCLUDED_CORE_I_LISTEN_CHILD_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_LISTEN_CHILD_DEATH_COMPONENT_H

#include "component.h"
#include <boost/serialization/export.hpp>

class IListenChildDeathComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IListenChildDeathComponent)
    virtual void SetKillerOfChildGUID(int32_t killerOfChildId)=0;
    virtual int32_t GetKillerOfChildGUID()const=0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void IListenChildDeathComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}


BOOST_CLASS_EXPORT_KEY2(IListenChildDeathComponent,"i_listen_child_death_component");
#endif//INCLUDED_CORE_I_LISTEN_CHILD_DEATH_COMPONENT_H
