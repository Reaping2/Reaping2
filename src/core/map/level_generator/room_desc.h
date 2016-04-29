#ifndef INCLUDED_MAP_ROOM_DESC_H
#define INCLUDED_MAP_ROOM_DESC_H

#include "platform/i_platform.h"

namespace map {

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
    void SetProperties( Properties_t& properties );
    Properties_t const& GetProperties() const;
    CellMatrix_t& GetCells();
protected:
    int32_t mCellCount = 0;
    int32_t mCellSize = 500;
    Properties_t mProperties;
    CellMatrix_t mCells;
};

} // namespace map

#endif//INCLUDED_MAP_ROOM_DESC_H
