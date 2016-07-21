#ifndef INCLUDED_MAP_CELL_ENTRANCE_PROPERTY_H
#define INCLUDED_MAP_CELL_ENTRANCE_PROPERTY_H

#include "core/map/level_generator/i_property.h"
#include "platform/i_platform.h"
#include "entrance_type.h"

namespace map {

class CellEntranceProperty : public IProperty
{
public:
    DEFINE_ROOM_PROPERTY_BASE(CellEntranceProperty)
    CellEntranceProperty( int32_t Id );
    virtual void Load( Json::Value& setters );
    virtual void Save( Json::Value& setters ) const;
    void SetX( int32_t x );
    int32_t GetX() const;
    void SetY( int32_t y );
    int32_t GetY() const;
    void SetEntranceType( EntranceType::Type type );
    EntranceType::Type GetEntranceType() const;
    typedef std::vector<int32_t> Targets_t;
    void SetBlockedTargets( Targets_t blockedTargets );
    Targets_t const& GetBlockedTargets() const;
    void SetEntranceTargets( Targets_t entranceTargets );
    Targets_t const& GetEntranceTargets() const;
    virtual void Generate( RoomDesc& roomDesc, MapElementHolder mMapElementHolder, glm::vec2 pos, bool editor = false );
private:
    int32_t mX;
    int32_t mY;
    EntranceType::Type mEntranceType;
    Targets_t mBlockedTargets;
    Targets_t mEntranceTargets;
};

} // namespace map

#endif//INCLUDED_MAP_CELL_ENTRANCE_PROPERTY_H

//command:  "classgenerator.exe" -g "map_element" -c "cell_entrance_property" -p "property" -n "map" -m "int32_t-x int32_t-y EntranceType::Type-type Targets_t-blockedTargets Targets_t-entranceTargets"
