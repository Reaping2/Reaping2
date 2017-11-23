#ifndef INCLUDED_MAP_CHEST_PROPERTY_H
#define INCLUDED_MAP_CHEST_PROPERTY_H

#include "core/map/level_generator/i_property.h"
#include "platform/i_platform.h"
#include "entrance_type.h"

namespace map {

class ChestProperty : public IProperty
{
public:
    DEFINE_ROOM_PROPERTY_BASE(ChestProperty)
    ChestProperty( int32_t Id );
    virtual void Load( Json::Value const& setters );
    virtual void Save( Json::Value& setters ) const;
    typedef std::vector<int32_t> Targets_t;
    void SetTargets( Targets_t blockedTargets );
    Targets_t const& GetTargets() const;
    virtual void Generate( RoomDesc& roomDesc, MapElementHolder& mMapElementHolder, glm::vec2 pos, bool editor = false );
private:
    Targets_t mTargets;
    double mChestX = 0.0;
    double mChestY = 0.0;
};

} // namespace map

#endif//INCLUDED_MAP_CHEST_PROPERTY_H

//command:  "classgenerator.exe" -g "map_element" -c "cell_entrance_property" -p "property" -n "map" -m "int32_t-x int32_t-y EntranceType::Type-type Targets_t-blockedTargets Targets_t-entranceTargets"
