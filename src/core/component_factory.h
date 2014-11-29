#ifndef INCLUDED_COMPONENT_REPO_H
#define INCLUDED_COMPONENT_REPO_H

#include "platform/factory.h"
#include "platform/singleton.h"

#include <memory>

class Component;

class ComponentFactory : public platform::Factory<Component>, public platform::Singleton<ComponentFactory>
{
    friend class platform::Singleton<ComponentFactory>;
    ComponentFactory();
protected:
    template<typename Elem_T>
    static std::auto_ptr<Component> CreateComponent( int32_t Id );
};
template<typename Elem_T>
std::auto_ptr<Component> ComponentFactory::CreateComponent( int32_t Id )
{
    return std::auto_ptr<Component>( new Elem_T() );
}

#endif//INCLUDED_COMPONENT_REPO_H