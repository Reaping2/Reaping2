#ifndef INCLUDED_CORE_LISTEN_CHILD_DEATH_COMPONENT_H
#define INCLUDED_CORE_LISTEN_CHILD_DEATH_COMPONENT_H

#include "i_listen_child_death_component.h"
#include "core/property_loader.h"
#include <boost/serialization/export.hpp>

class ListenChildDeathComponent : public IListenChildDeathComponent
{
public:
    ListenChildDeathComponent();
    virtual void SetKillerOfChildGUID(int32_t killerOfChildId);
    virtual int32_t GetKillerOfChildGUID()const;
protected:
    friend class ComponentFactory;
    int32_t mKillerOfChildGUID;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void ListenChildDeathComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<IListenChildDeathComponent>(*this);
    ar & mKillerOfChildGUID;
}

class ListenChildDeathComponentLoader : public ComponentLoader<ListenChildDeathComponent>
{
    virtual void BindValues();
protected:
    ListenChildDeathComponentLoader();
    friend class ComponentLoaderFactory;
};


BOOST_CLASS_EXPORT_KEY2(ListenChildDeathComponent,"listen_child_death_component");
#endif//INCLUDED_CORE_LISTEN_CHILD_DEATH_COMPONENT_H
