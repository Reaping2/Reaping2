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
    virtual void Load(Json::Value& setters)=0;
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

    template<class VALUE_T>
    void Bind(boost::function<void (COMPONENT*,VALUE_T)> func, VALUE_T val);
    typedef boost::function<void (COMPONENT*,int32_t)> func_int32_t;
    void Bind(std::string const& val, func_int32_t func);
    typedef boost::function<void (COMPONENT*,double)> func_double;
    void Bind(std::string const& val, func_double func);

    virtual void Load(Json::Value& setters);
    virtual void BindValues()=0;

	virtual std::auto_ptr<Component> LoadComponent(std::auto_ptr<Component> component)const;
    virtual ~ComponentLoader();
protected:
    Json::Value* mSetters;
    ComponentLoader();
    friend class ComponentLoaderFactory;
    void Add(SetterFunc_t func);
};

template<class COMPONENT>
template<class VALUE_T>
void ComponentLoader<COMPONENT>::Bind(boost::function<void (COMPONENT*,VALUE_T)> func, VALUE_T val)
{
    Add(boost::bind(func,_1,val));
}

template<class COMPONENT>
void ComponentLoader<COMPONENT>::Bind(std::string const& val,func_int32_t func)
{
    int32_t iv;
    if( Json::GetInt( (*mSetters)[val], iv))
    {
        Bind<int32_t>(func,iv);
    }
}

template<class COMPONENT>
void ComponentLoader<COMPONENT>::Bind(std::string const& val,func_double func)
{
    double dv;
    if( Json::GetDouble( (*mSetters)[val], dv))
    {
        Bind<double>(func,dv);
    }
}

template<typename COMPONENT>
void ComponentLoader<COMPONENT>::Load(Json::Value& setters)
{
    mSetters=&setters;
    BindValues();
}



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
    for(typename SetterFuncList_t::const_iterator i=mSetterFuncList.begin(),e=mSetterFuncList.end();i!=e;++i)
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
    virtual void BindValues() {}
protected:
    DefaultComponentLoader() {}
    friend class ComponentLoaderFactory;
};


#endif//INCLUDED_CORE_COMPONENT_LOADER_H