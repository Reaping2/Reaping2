#ifndef INCLUDED_MAP_ROOM_DESC_H
#define INCLUDED_MAP_ROOM_DESC_H

#include "platform/i_platform.h"
#include "core/opt.h"

namespace map {

class IRoom;
// describes a room cell
struct Cell 
{
    enum Entrance
    {
        Top=0,
        Right,
        Bottom,
        Left,
        Num_entrances
    };
    typedef std::set<Entrance> Entrances_t;
    Entrances_t mEntrances; // possible entrances to this cell
    bool mFilled = false; // filled if the correspoinding RoomDesc places stg into this cell
};

// describes an n*n room.
// can contain empty cells that could be filled later
struct RoomDesc
{
    enum Property
    {
        Start = 0,
        End,
        Key,
        Num_Properties
    };
    typedef std::vector<std::vector<Cell>> CellMatrix_t;
    typedef std::set<Property> Properties_t;
    RoomDesc();
    void SetCellCount( int32_t cellCount );
    int32_t GetCellCount() const;
    void SetCellSize( int32_t cellSize );
    int32_t GetCellSize();
    Properties_t& GetProperties();
    CellMatrix_t& GetCells();
    void ClearAllCellEntrances();
    Opt<IRoom> GetRoom();
    void SetRoom( Opt<IRoom> room );

    typedef std::vector<int32_t> PlacedActorGUIDs_t;
    PlacedActorGUIDs_t mPlacedActorGUIDs; // for debug
protected:
    int32_t mCellCount = 0;
    int32_t mCellSize = 500;
    Properties_t mProperties;
    CellMatrix_t mCells;
    Opt<IRoom> mRoom;
};

} // namespace map

#endif//INCLUDED_MAP_ROOM_DESC_H
