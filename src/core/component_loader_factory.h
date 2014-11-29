#ifndef INCLUDED_CORE_COMPONENT_LOADER_FACTORY_H
#define INCLUDED_CORE_COMPONENT_LOADER_FACTORY_H

#include "platform/factory.h"
#include "platform/singleton.h"
#include "core/property_loader.h"
#include "i_core.h"

template<typename BASE>
class PropertyLoaderBase;
class ComponentLoaderFactory : public platform::Factory<PropertyLoaderBase<Component> >, public platform::Singleton<ComponentLoaderFactory>
{
    friend class platform::Singleton<ComponentLoaderFactory>;
    ComponentLoaderFactory();
    template<typename Elem_T>
    static std::auto_ptr<PropertyLoaderBase<Component> > CreateComponentLoader( int32_t Id );
};
template<typename Elem_T>
std::auto_ptr<PropertyLoaderBase<Component> > ComponentLoaderFactory::CreateComponentLoader( int32_t Id )
{
    return std::auto_ptr<PropertyLoaderBase<Component> >( new Elem_T() );
}

class DefaultComponentLoader: public ComponentLoader<Component>
{
public:
    virtual void BindValues();
protected:
    DefaultComponentLoader();
    friend class ComponentLoaderFactory;
};



#endif//INCLUDED_CORE_COMPONENT_LOADER_FACTORY_H