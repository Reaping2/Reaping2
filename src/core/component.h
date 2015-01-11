#ifndef INCLUDED_CORE_COMPONENT_H
#define INCLUDED_CORE_COMPONENT_H

#include "core/component_factory.h"
#include "core/opt.h"
#include "platform/auto_id.h"
#include <boost/ptr_container/ptr_map.hpp>

#define DEFINE_COMPONENT_BASE( ComponentType ) \
    static int GetType_static() \
    { \
        static int const typ = platform::AutoId( #ComponentType ); \
        return typ; \
    } \
    virtual int GetType() const \
    { \
        return ComponentType::GetType_static(); \
    } \


class Actor;

class Component 
{
public:
    virtual int GetType() const=0;
    virtual ~Component();
    virtual void SetActor(Actor* Obj);
    void SetId(int32_t id);
    int32_t GetId() const;
protected:
    Actor* mActor;
    int32_t mId;
    Component();
};

class ComponentHolder
{
protected:
    typedef boost::ptr_map<int32_t, Component> ComponentList_t;
    ComponentFactory& mComponentFactory;
    ComponentList_t mComponents;

public:
    ComponentHolder();
    virtual void AddComponent( std::auto_ptr<Component> Comp  );
   
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
    return Opt<Component_t>(dynamic_cast<Component_t*>(const_cast<Component*>(
        i == mComponents.end()?NULL:i->second))); 
}

template<typename Component_t>
Opt<Component_t> ComponentHolder::Get()
{
    return ( ( const ComponentHolder* )this )->Get<Component_t>();
}

class DefaultComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(DefaultComponent)
    DefaultComponent();
    friend class ComponentFactory;
};

template<typename T,typename BASE>
class PropertyLoader;
template<typename COMPONENT>
class ComponentLoader: public PropertyLoader<COMPONENT, Component>
{
};
#endif//INCLUDED_CORE_COMPONENT_H
