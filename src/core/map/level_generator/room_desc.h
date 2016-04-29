#ifndef INCLUDED_MAP_ROOM_DESC_H
#define INCLUDED_MAP_ROOM_DESC_H

#include "platform/i_platform.h"
#include "core/opt.h"

namespace map {

class IRoom;
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
    Entrances_t mEntrances;
    bool mFilled = false;
};

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
    typedef std::vector<Property> Properties_t;
    RoomDesc();
    void SetCellCount( int32_t cellCount );
    int32_t GetCellCount() const;
    void SetCellSize( int32_t cellSize );
    int32_t GetCellSize();
    Properties_t& GetProperties();
    CellMatrix_t& GetCells();
    void ClearAllCellentrances();
    Opt<IRoom> GetRoom();
    void SetRoom( Opt<IRoom> room );
protected:
    int32_t mCellCount = 0;
    int32_t mCellSize = 500;
    Properties_t mProperties;
    CellMatrix_t mCells;
    Opt<IRoom> mRoom;
};

} // namespace map

#endif//INCLUDED_MAP_ROOM_DESC_H
