#ifndef INCLUDED_MAP_I_PROPERTY_H
#define INCLUDED_MAP_I_PROPERTY_H

#include "platform/i_platform.h"
#include "../map_system.h"
#include "room_desc.h"

namespace map {

#define DEFINE_ROOM_PROPERTY_BASE( PropType ) \
    static int GetType_static() \
    { \
        static int const typ = platform::AutoId( #PropType ); \
        return typ; \
    } \
    virtual int GetType() const \
    { \
        return PropType::GetType_static(); \
    } \

class IProperty
{
public:
	IProperty( int32_t Id );
	virtual ~IProperty() = default;
    virtual void Load( Json::Value& setters );
    virtual void Save( Json::Value& setters ) const;
    virtual int GetType() const = 0;
	int32_t GetId() const;
    virtual void Generate( RoomDesc& roomDesc, MapElementHolder mMapElementHolder, glm::vec2 pos, bool editor = false );
    int32_t GetUID() const;
protected:
	int32_t mId;
    int32_t mUID;
};

class DefaultProperty : public IProperty
{
public:
    DEFINE_ROOM_PROPERTY_BASE( DefaultProperty )
    DefaultProperty( int32_t Id );
};

} // namespace map

#endif INCLUDED_MAP_I_PROPERTY_H

//command:  "classgenerator.exe" -g "component" -c "i_property" -n "map" -m "int32_t-id"
