#ifndef INCLUDED_CORE_COMPONENT_H
#define INCLUDED_CORE_COMPONENT_H

#include "core/component_repo.h"
#include "core/opt.h"
#include "platform/auto_id.h"

#include <boost/ptr_container/ptr_map.hpp>

#define DEFINE_COMPONENT_BASE( ComponentType ) \
    static int GetType_static() \
    { \
        static int const typ = platform::AutoId( #ComponentType ); \
        return typ; \
    } \
    virtual int GetType_interface() const \
    { \
        return ComponentType::GetType_static(); \
    } \

#define DEFINE_COMPONENT_IMPLEMENTATION( ComponentType ) \
    static int GetType() \
    { \
    static int const typ = platform::AutoId( #ComponentType ); \
    return typ; \
    } \

class Component 
{
public:
    virtual int GetType_interface() const=0;
    virtual ~Component();

protected:
    Component();
};

class ComponentHolder
{
protected:
    typedef boost::ptr_map<int32_t, Component> ComponentList_t;
    ComponentRepo& mComponentFactory;
    ComponentList_t mComponents;

public:
    ComponentHolder();
    void AddComponent( std::auto_ptr<Component> Comp  );
   
    template<typename Component_t>
    Opt<Component_t> Get() const;
    template<typename Component_t>
    Opt<Component_t> Get();
    virtual ~ComponentHolder();
};
template<typename Component_t>
Opt<Component_t> ComponentHolder::Get() const
{
    ComponentList_t::const_iterator i = mComponents.find( Component_t::GetType_static() );
    BOOST_ASSERT(i != mComponents.end()); // if this one is not true the we a screwed
    return (Opt<Component_t>(static_cast<Component_t*>(const_cast<Component*>(i->second)))); 
}

template<typename Component_t>
Opt<Component_t> ComponentHolder::Get()
{
    return ( ( const ComponentHolder* )this )->Get<Component_t>();
}

class DefaultComponent : public Component
{
public:
    DEFINE_COMPONENT_IMPLEMENTATION(DefaultComponent)
    DEFINE_COMPONENT_BASE(DefaultComponent)
    DefaultComponent();
    friend class ComponentRepo;
};
#endif//INCLUDED_CORE_COMPONENT_H
