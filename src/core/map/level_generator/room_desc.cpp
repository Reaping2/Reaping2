#include "room_desc.h"
#include "i_room.h"
#include "platform/id_storage.h"
#include "room_property.h"

namespace map {


RoomDesc::RoomDesc()
{

}

void RoomDesc::SetCellCount( int32_t cellCount )
{
    mCellCount = cellCount;
    mCells.resize( mCellCount );
    for (int32_t y = 0; y < mCellCount; ++y)
    {
        mCells[y].resize( mCellCount );
        for (int32_t x = 0; x < mCellCount; ++x)
        {
            mCells[y][x].mDescCoord = glm::vec2( x, y );
        }
    }
}

int32_t RoomDesc::GetCellCount() const
{
    return mCellCount;
}

void RoomDesc::SetCellSize( int32_t cellSize )
{
    mCellSize = cellSize;
}

int32_t RoomDesc::GetCellSize() const
{
    return mCellSize;
}

RoomDesc::PlainProperties_t const& RoomDesc::GetPlainProperties() const
{
    return mPossibleProperties;
}


void RoomDesc::SetPlainProperties( PlainProperties_t const& properties )
{
    mPossibleProperties = properties;
}


bool RoomDesc::HasProperty( RoomProperty::Type prop ) const
{
    return prop==RoomProperty::Nothing||mPossibleProperties.find( prop ) != mPossibleProperties.end();
}

void RoomDesc::ClearProperties()
{
    mPossibleProperties.clear();
}


void RoomDesc::AddProperty( RoomProperty::Type prop )
{
    mPossibleProperties.insert( prop );
}


void RoomDesc::RemoveProperty( RoomProperty::Type prop )
{
    mPossibleProperties.erase( prop );
}

map::Cell& RoomDesc::GetCell( int32_t x, int32_t y )
{
    return mCells[y][x];
}


map::Cell const& RoomDesc::GetCell( int32_t x, int32_t y ) const
{
    return mCells[y][x];
}


map::Cell& RoomDesc::GetCell( glm::vec2 pos )
{
    return mCells[pos.y][pos.x];
}


map::Cell const& RoomDesc::GetCell( glm::vec2 pos ) const
{
    return mCells[pos.y][pos.x];
}

bool RoomDesc::IsFilled( int32_t x, int32_t y ) const
{
    return GetCell( x, y ).mFilled;
}


bool RoomDesc::IsFilled( glm::vec2 pos ) const
{
    return GetCell( pos ).mFilled;
}

void RoomDesc::ClearCellEntrances()
{
    for (auto& row : mCells)
    {
        for (auto& cell : row)
        {
            cell.mPossibleEntrances.clear();
        }
    }
}

Opt<IRoom> RoomDesc::GetRoom() const
{
    return mRoom;
}


void RoomDesc::SetRoom( Opt<IRoom> room )
{
    mRoom = room;
}


void RoomDesc::Load( Json::Value const& setters )
{
    mPossibleProperties.clear();
    Json::Value const& properties = setters["plain_properties"];
    if (properties.isArray())
    {
        for (auto&& prop : properties)
        {
            AddProperty( RoomProperty::Get()(AutoId( prop.asString() )) );
        }
    }
    int i = 0;
    if (Json::GetInt( setters["cell_count"], i ))
    {
        SetCellCount( i );
    }
    if (Json::GetInt( setters["cell_size"], i ))
    {
        SetCellSize( i );
    }
    auto& cells = setters["cells"];
    if (cells.isArray())
    {
        for (auto& cell : cells)
        {
            int x = 0;
            int y = 0;
            if (Json::GetInt( cell["x"], x ) && Json::GetInt( cell["y"], y ))
            {
                GetCell( x, y ).Load( cell );
            }
        }
    }
}


void RoomDesc::Save( Json::Value& setters ) const
{
    auto& idStorage = IdStorage::Get();
    auto& roomProperty = RoomProperty::Get();
    Json::Value plainPropertyArray( Json::arrayValue );
    for (auto&& prop : mPossibleProperties)
    {
        std::string propName;
        if (idStorage.GetName( roomProperty( prop ), propName ))
        {
            Json::Value jName = Json::Value( propName );
            plainPropertyArray.append( jName );
        }
    }
    setters["plain_properties"] = plainPropertyArray;
    setters["cell_count"] = mCellCount;
    setters["cell_size"] = mCellSize;
    Json::Value cellsArray( Json::arrayValue );
    for (auto& cellRow : mCells)
    {
        for (auto& cell : cellRow)
        {
            if (cell.IsFilled())
            {
                Json::Value cellObject( Json::objectValue );
                cell.Save( cellObject );
                cellsArray.append( cellObject );
            }
        }
    }
    setters["cells"] = cellsArray;
}


bool RoomDesc::FitsInto( RoomDesc const& roomDesc, int32_t flags ) const
{
    bool r = true;
    if (r && flags&Layout != 0)
    {
        if (mCellCount != roomDesc.GetCellCount())
        {
            r = false;
        }
        const bool checkEntrance = flags&Entrance != 0;
        int32_t y = 0;
        while (r && y < mCellCount)
        {
            int32_t x = 0;
            while (r && x < mCellCount)
            {
                auto const& cell = GetCell( x, y );
                auto const& otherCell = roomDesc.GetCell( x, y );
                if (cell.IsFilled() == otherCell.IsFilled())
                {
                    if (checkEntrance)
                    {
                        for (auto entrance : cell.GetEntrances())
                        {
                            if (!otherCell.HasEntrance( entrance ))
                            {
                                r = false;
                                break;
                            }
                                
                        }
                    }
                }
                else
                {
                    r = false;
                }
                ++x;
            }
            ++y;
        }
    }
    if (r && flags&Properties != 0)
    {
        for (auto prop : mPossibleProperties)
        {
            if (!roomDesc.HasProperty( prop ))
            {
                r = false;
                break;
            }
        }
    }
    return r;
}

void Cell::AddEntrance( EntranceType::Type const& entrance )
{
    mPossibleEntrances.insert( entrance );
}


void Cell::RemoveEntrance( EntranceType::Type const& entrance )
{
    mPossibleEntrances.erase( entrance );
}

void Cell::SetEntrances( Entrances_t const& entrances )
{
    mPossibleEntrances = entrances;
}


bool Cell::HasEntrance( EntranceType::Type const& entrance ) const
{
    return mPossibleEntrances.find( entrance ) != mPossibleEntrances.end();
}


Cell::Entrances_t const& Cell::GetEntrances() const
{
    return mPossibleEntrances;
}

void Cell::SetFilled( bool filled )
{
    mFilled = filled;
}


bool Cell::IsFilled() const
{
    return mFilled;
}


void Cell::Load( Json::Value const& setters )
{
    int i = 0;
    if (Json::GetInt( setters["x"], i ))
    {
        mDescCoord.x = i;
    }
    if (Json::GetInt( setters["y"], i ))
    {
        mDescCoord.y = i;
    }
    mPossibleEntrances.clear();
    auto const& entrances = setters["entrances"];
    if (entrances.isArray())
    {
        for (auto& entrance : entrances)
        {
            AddEntrance( EntranceType::Get()(AutoId( entrance.asString() )) );
        }
    }
    mFilled = true;
}


void Cell::Save( Json::Value& setters ) const
{
    auto& idStorage = IdStorage::Get();
    auto& entranceType = EntranceType::Get();
    Json::Value entrances( Json::arrayValue );
    for (auto& entrance : mPossibleEntrances)
    {
        std::string entranceName;
        if (idStorage.GetName( entranceType( entrance ), entranceName ))
        {
            Json::Value jName = Json::Value( entranceName );
            entrances.append( jName );
        }
    }
    setters["entrances"] = entrances;
    setters["x"] = mDescCoord.x;
    setters["y"] = mDescCoord.y;
    setters["name"] = Json::Value("cell");
}

} // namespace map
