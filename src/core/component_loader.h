#ifndef INCLUDED_CORE_COMPONENT_LOADER_H
#define INCLUDED_CORE_COMPONENT_LOADER_H

#include "platform/factory.h"
#include "platform/singleton.h"
#include <boost/ptr_container/ptr_map.hpp>
#include "core/component.h"
#include "core/component_factory.h"
#include <list>
#include <string>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "core/component.h"
#include <boost/static_assert.hpp>
#include "core/component_loader_factory.h"
#include "i_core.h"

class ComponentLoaderBase
{
public:
    virtual ~ComponentLoaderBase();
    virtual void LoadValues(Json::Value& setters)=0;
    virtual std::auto_ptr<Component> LoadComponent(std::auto_ptr<Component> component)const=0;
protected:
    friend class ComponentLoaderFactory;
};

template<typename COMPONENT>
class ComponentLoader:public ComponentLoaderBase
{
    BOOST_STATIC_ASSERT_MSG(
        ( boost::is_base_of<Component, COMPONENT>::value ),
        "Component must be a descendant of Component!"
        );
public:
    typedef boost::function<void (COMPONENT*)> SetterFunc_t;
   	typedef std::list<SetterFunc_t > SetterFuncList_t;
	SetterFuncList_t mSetterFuncList;

    template<typename VALUE_T>
    void Bind(boost::function<void (COMPONENT*,VALUE_T)> func, VALUE_T val)
    {
        Add(boost::bind(func,_1,val));
    }
	virtual std::auto_ptr<Component> LoadComponent(std::auto_ptr<Component> component)const;
    virtual ~ComponentLoader();
protected:
    ComponentLoader();
    friend class ComponentLoaderFactory;
    void Add(SetterFunc_t func);
};

template<typename COMPONENT>
ComponentLoader<COMPONENT>::~ComponentLoader()
{

}

template<typename COMPONENT>
ComponentLoader<COMPONENT>::ComponentLoader()
{
}

template<typename COMPONENT>
std::auto_ptr<Component> ComponentLoader<COMPONENT>::LoadComponent(std::auto_ptr<Component> component)const
{
    if(mSetterFuncList.empty())
        return component;
    COMPONENT* castedComponent=static_cast<COMPONENT*>(component.release());
    for(SetterFuncList_t::const_iterator i=mSetterFuncList.begin(),e=mSetterFuncList.end();i!=e;++i)
    {
        (*i)(castedComponent);
    }
    return std::auto_ptr<Component>(static_cast<Component*>(castedComponent));
}

template<typename COMPONENT>
void ComponentLoader<COMPONENT>::Add(SetterFunc_t func)
{
    mSetterFuncList.push_back(func);
}

class DefaultComponentLoader: public ComponentLoader<DefaultComponent>
{
public:
    virtual void LoadValues(Json::Value& setters) {}
protected:
    DefaultComponentLoader() {}
    friend class ComponentLoaderFactory;
};


#endif//INCLUDED_CORE_COMPONENT_LOADER_H