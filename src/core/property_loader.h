#ifndef INCLUDED_CORE_PROPERTY_LOADER_H
#define INCLUDED_CORE_PROPERTY_LOADER_H

#include "platform/factory.h"
#include "platform/singleton.h"
#include <boost/ptr_container/ptr_map.hpp>
#include "core/component.h"
#include "core/component_factory.h"
#include <list>
#include <string>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/static_assert.hpp>
#include "i_core.h"

template<typename BASE>
class PropertyLoaderBase
{
public:
    virtual ~PropertyLoaderBase();
    virtual void Load(Json::Value& setters)=0;
    virtual std::auto_ptr<BASE> FillProperties(std::auto_ptr<BASE> target)const=0;
};

template<typename BASE>
PropertyLoaderBase<BASE>::~PropertyLoaderBase()
{

}


template<typename T, typename BASE>
class PropertyLoader:public PropertyLoaderBase<BASE>
{
    BOOST_STATIC_ASSERT_MSG(
        ( boost::is_base_of<BASE, T>::value ),
        "First template arg T must be a descendant of BASE"
        );
public:
    typedef boost::function<void (T*)> SetterFunc_t;
   	typedef std::list<SetterFunc_t > SetterFuncList_t;
	SetterFuncList_t mSetterFuncList;

    template<class VALUE_T>
    void Bind(boost::function<void (T*,VALUE_T)> func, VALUE_T val);
    typedef boost::function<void (T*,int32_t)> func_int32_t;
    void Bind(std::string const& val, func_int32_t func);
    typedef boost::function<void (T*,double)> func_double;
    void Bind(std::string const& val, func_double func);

    virtual void Load(Json::Value& setters);
    virtual void BindValues()=0;

	virtual std::auto_ptr<BASE> FillProperties(std::auto_ptr<BASE> target)const;
    virtual ~PropertyLoader();
protected:
    Json::Value* mSetters;
    PropertyLoader();
    void Add(SetterFunc_t func);
};
template<typename T, typename BASE>
std::auto_ptr<BASE> PropertyLoader<T, BASE>::FillProperties(std::auto_ptr<BASE> target)const
{
    if(mSetterFuncList.empty())
        return target;
    T* castedTarget=static_cast<T*>(target.release());
    for(typename SetterFuncList_t::const_iterator i=mSetterFuncList.begin(),e=mSetterFuncList.end();i!=e;++i)
    {
        (*i)(castedTarget);
    }
    return std::auto_ptr<BASE>(static_cast<BASE*>(castedTarget));
}

template<typename T, typename BASE>
template<class VALUE_T>
void PropertyLoader<T, BASE>::Bind(boost::function<void (T*,VALUE_T)> func, VALUE_T val)
{
    Add(boost::bind(func,_1,val));
}

template<typename T, typename BASE>
void PropertyLoader<T, BASE>::Bind(std::string const& val,func_int32_t func)
{
    int32_t iv;
    if( Json::GetInt( (*mSetters)[val], iv))
    {
        Bind<int32_t>(func,iv);
    }
}

template<typename T, typename BASE>
void PropertyLoader<T, BASE>::Bind(std::string const& val,func_double func)
{
    double dv;
    if( Json::GetDouble( (*mSetters)[val], dv))
    {
        Bind<double>(func,dv);
    }
}

template<typename T, typename BASE>
void PropertyLoader<T, BASE>::Load(Json::Value& setters)
{
    mSetters=&setters;
    BindValues();
}



template<typename T, typename BASE>
PropertyLoader<T, BASE>::~PropertyLoader()
{

}

template<typename T, typename BASE>
PropertyLoader<T, BASE>::PropertyLoader()
{
}



template<typename T, typename BASE>
void PropertyLoader<T, BASE>::Add(SetterFunc_t func)
{
    mSetterFuncList.push_back(func);
}



#endif//INCLUDED_CORE_PROPERTY_LOADER_H