#ifndef INCLUDED_CORE_COMPONENT_LOADER_FACTORY_H
#define INCLUDED_CORE_COMPONENT_LOADER_FACTORY_H

#include "platform/factory.h"
#include "platform/singleton.h"
#include "core/component_loader.h"
#include "i_core.h"

class ComponentLoaderBase;
class ComponentLoaderFactory : public platform::Factory<ComponentLoaderBase>, public platform::Singleton<ComponentLoaderFactory>
{
    friend class platform::Singleton<ComponentLoaderFactory>;
    ComponentLoaderFactory();
    template<typename Elem_T>
    static std::auto_ptr<ComponentLoaderBase> CreateComponent( int32_t Id );
};
template<typename Elem_T>
std::auto_ptr<ComponentLoaderBase> ComponentLoaderFactory::CreateComponent( int32_t Id )
{
    return std::auto_ptr<ComponentLoaderBase>( new Elem_T() );
}


#endif//INCLUDED_CORE_COMPONENT_LOADER_FACTORY_H