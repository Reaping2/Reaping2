#include "core/map/level_generator/cell_entrance_property.h"
#include "spawn_property.h"


namespace map {

CellEntranceProperty::CellEntranceProperty( int32_t Id )
    : IProperty( Id )
    , mX( 0 )
    , mY( 0 )
    , mEntranceType( EntranceType::Top )
    , mBlockedTargets()
    , mEntranceTargets()
{
}

void CellEntranceProperty::Load( Json::Value& setters )
{
    IProperty::Load( setters );
    int i = 0;
    if (Json::GetInt( setters["x"], i ))
    {
        mX = i;
    }
    if (Json::GetInt( setters["y"], i ))
    {
        mY = i;
    }
    mBlockedTargets.clear();
    auto const& blockedTargets = setters["blocked_targets"];
    if (blockedTargets.isArray())
    {
        for (auto& blockedTarget : blockedTargets)
        {
            mBlockedTargets.push_back( AutoId( blockedTarget.asString() ) );
        }
    }
    mEntranceTargets.clear();
    auto const& entranceTargets = setters["entrance_targets"];
    if (entranceTargets.isArray())
    {
        for (auto& entranceTarget : entranceTargets)
        {
            mEntranceTargets.push_back( AutoId( entranceTarget.asString() ) );
        }
    }
    std::string str;
    if (Json::GetStr( setters["type"], str ))
    {
        mEntranceType = EntranceType::Get()(AutoId( str ));
    }
}

void CellEntranceProperty::SetX( int32_t x )
{
    mX = x;
}

int32_t CellEntranceProperty::GetX() const
{
    return mX;
}

void CellEntranceProperty::SetY( int32_t y )
{
    mY = y;
}

int32_t CellEntranceProperty::GetY() const
{
    return mY;
}

void CellEntranceProperty::SetEntranceType( EntranceType::Type type )
{
    mEntranceType = type;
}

EntranceType::Type CellEntranceProperty::GetEntranceType() const
{
    return mEntranceType;
}

void CellEntranceProperty::SetBlockedTargets( Targets_t blockedTargets )
{
    mBlockedTargets = blockedTargets;
}

CellEntranceProperty::Targets_t const& CellEntranceProperty::GetBlockedTargets() const
{
    return mBlockedTargets;
}

void CellEntranceProperty::SetEntranceTargets( Targets_t entranceTargets )
{
    mEntranceTargets = entranceTargets;
}

CellEntranceProperty::Targets_t const& CellEntranceProperty::GetEntranceTargets() const
{
    return mEntranceTargets;
}


void CellEntranceProperty::Generate( RoomDesc& roomDesc, MapElementHolder mMapElementHolder, glm::vec2 pos )
{
    if (roomDesc.GetCell(mX,mY).HasEntrance( mEntranceType ))
    {
        SpawnProperty::SpawnTargets( roomDesc, mEntranceTargets, mMapElementHolder, pos );
    }
    else
    {
        SpawnProperty::SpawnTargets( roomDesc, mBlockedTargets, mMapElementHolder, pos );
    }
}

} // namespace map
