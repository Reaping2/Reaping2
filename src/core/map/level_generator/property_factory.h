#ifndef INCLUDED_MAP_PROPERTY_FACTORY_H
#define INCLUDED_MAP_PROPERTY_FACTORY_H

#include "platform/factory.h"
#include "platform/singleton.h"
#include "i_property.h"

namespace map {

class PropertyFactory : public platform::Factory<IProperty>, public platform::Singleton<PropertyFactory>
{
    friend class platform::Singleton<PropertyFactory>;
    PropertyFactory();
protected:
    template<typename Elem_T>
    static std::auto_ptr<IProperty> CreateIProperty( int32_t Id );
};

template<typename Elem_T>
std::auto_ptr<IProperty> PropertyFactory::CreateIProperty( int32_t Id )
{
    return std::auto_ptr<IProperty>( new Elem_T( Id ) );
}

} // namespace map

#endif//INCLUDED_MAP_PROPERTY_FACTORY_H

//command:  "classgenerator.exe" -g "factory" -c "property_factory" -t "i_property" -n "map"
