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
    Entrances_t mPossibleEntrances; // possible entrances to this cell
    bool mFilled = false; // filled if the correspoinding RoomDesc places stg into this cell
    glm::vec2 mDescCoord = glm::vec2( -1, -1 ); // relative position inside the room
    void AddEntrance( Entrance const& entrance );
    void SetEntrances( Entrances_t const& entrances );
    bool HasEntrance( Entrance const& entrance) const;
    Entrances_t const& GetEntrances() const;
    void SetFilled( bool filled );
    bool IsFilled();
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
    int32_t GetCellSize() const;
    Properties_t const& GetProperties() const;
    void SetProperties( Properties_t const& properties );
    bool HasProperty( Property prop ) const;
    void ClearProperties();
    void AddProperty( Property prop );
    Cell& GetCell( int32_t x, int32_t y );
    Cell& GetCell( glm::vec2 pos );
    Cell const& GetCell( int32_t x, int32_t y ) const;
    Cell const& GetCell( glm::vec2 pos ) const;
    bool IsFilled( int32_t x, int32_t y ) const;
    bool IsFilled( glm::vec2 pos ) const;
    void ClearCellEntrances();
    Opt<IRoom> GetRoom();
    void SetRoom( Opt<IRoom> room );

    typedef std::vector<int32_t> PlacedActorGUIDs_t;
    PlacedActorGUIDs_t mPlacedActorGUIDs; // for debug
protected:
    int32_t mCellCount = 0;
    int32_t mCellSize = 500;
    Properties_t mPossibleProperties;
    CellMatrix_t mCells;
    Opt<IRoom> mRoom;
};

} // namespace map

#endif//INCLUDED_MAP_ROOM_DESC_H
