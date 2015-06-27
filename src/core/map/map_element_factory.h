#ifndef INCLUDED_CORE_MAP_ELEMENT_FACTORY_H
#define INCLUDED_CORE_MAP_ELEMENT_FACTORY_H

#include "platform/factory.h"
#include "platform/singleton.h"
#include "map_element.h"

namespace map {

class MapElementFactory : public platform::Factory<MapElement>, public platform::Singleton<MapElementFactory>
{
    friend class platform::Singleton<MapElementFactory>;
    MapElementFactory();
protected:
    template<typename Elem_T>
    static std::auto_ptr<MapElement> CreateMapElement( int32_t Id );
};

template<typename Elem_T>
std::auto_ptr<MapElement> MapElementFactory::CreateMapElement( int32_t Id )
{
    return std::auto_ptr<MapElement>( new Elem_T() );
}

} // namespace map

#endif//INCLUDED_CORE_MAP_ELEMENT_FACTORY_H
