#ifndef INCLUDED_MAP_GENERATOR_DATA_H
#define INCLUDED_MAP_GENERATOR_DATA_H

#include "core/actor_factory.h"
#include "platform/i_platform.h"
#include "i_room.h"
#include "room_property.h"
#include "possible_rooms.h"

namespace map {

// a single cell describing the corresponding room
struct GCell
{
    int32_t mGRoomDescIndex = -1;
    bool mFilled = false; // filled if a room places something into this cell
};

struct GRoomDesc
{
    RoomDesc mRoomDesc;
    bool mIsReplaceable = true;
    PossibleRooms mPossibleRooms;
    glm::vec2 mRoomCoord = glm::vec2( -1, -1 ); // absolute position in mGCells
};
typedef std::vector<int32_t> NeighbourRooms_t;
// node for one particular placed room
struct GGraphNode
{
    int32_t mIndex; // index to mGRoomDescs
    NeighbourRooms_t mNeighbours; // indices to mGRoomDescs
    GGraphNode( int32_t ind, NeighbourRooms_t const& nodes );
    int32_t operator[]( int32_t neighbourIndex ) const;
    int32_t Size() const;
    void ShuffleNeighbours();
};

// graph of rooms
struct GGraph
{
    std::vector<GGraphNode> mNodes; // same order and count as mGRoomDescs
    void ShuffleNodeNeighbours();
    GGraphNode const& operator[]( int32_t nodeIndex ) const;
    void Clear();
    void AddNode( GGraphNode const& node );
};

class GeneratorData
{
public:
    GeneratorData();
    GCell& GetGCell( glm::vec2 pos );
    GCell const& GetGCell( glm::vec2 pos ) const;
    void SetDimensions( int32_t x, int32_t y );
    bool IsFilled( glm::vec2 pos ) const;
    bool IsRoomIdentical( glm::vec2 pos, int32_t roomIndex ) const;
    void PlaceRoom( RoomDesc const& roomDesc, glm::vec2 pos, PossibleRooms const& possibleRooms );
    void ReplaceRoom( int32_t roomIndex, int32_t roomId );
    bool CanPlaceRoom( RoomDesc const& roomDesc, glm::vec2 pos, PossibleRooms const& possibleRooms ) const;
    bool IsInBounds( glm::vec2 pos ) const;
    void ClearRoomProperties( int32_t roomIndex );
    void AddRoomProperty( int32_t roomIndex, RoomProperty::Type prop );
    int32_t GetRoomCount() const;
    Opt<IRoom> GetRoom( int32_t roomIndex );
    RoomDesc const& GetRoomDesc( int32_t roomIndex) const;
    RoomDesc& GetRoomDesc( int32_t roomIndex );
    glm::vec2 GetRoomCoord( int32_t roomIndex ) const;
    void LinkCells( glm::vec2 posA, glm::vec2 posB );
    bool CanLinkCells( glm::vec2 posA, glm::vec2 posB, bool replaceableA = false, bool replaceableB = false ) const;
    bool AreCellsLinked( glm::vec2 posA, glm::vec2 posB ) const;
    void ShuffleNeighbours();
    int32_t GetNeighbourRoomCount( int32_t roomIndex ) const;
    int32_t GetNeigbourRoomIndex( int32_t roomIndex, int32_t neighbourIndex ) const;
    NeighbourRooms_t const& GetNeighbourRooms( int32_t roomIndex ) const;
    typedef std::pair<glm::vec2, glm::vec2> CellPair_t;
    typedef std::vector<CellPair_t> CellPairs_t;
    CellPairs_t GetAdjacentCellPairs( int32_t roomA, int32_t roomB );
    GRoomDesc const& GetGRoomDesc( int32_t roomIndex ) const;
    bool HasGRoomDesc( glm::vec2 pos ) const;
    bool HasUnreplaceableNeighbor( IRoom const& room, glm::vec2 pos ) const;
    GGraph const& GetGraph() const;
    // generates graph with all poosible neighbour rooms. Ignores current entrances.
    void GenerateGraph();
    // generates graph with neighbours respecting current entrances
    void GenerateRouteGraph();
private:
    typedef std::vector<std::vector<GCell>> GCellMatrix_t;
    GCellMatrix_t mGCells; // all cells with the corresponding roomDesc
    int32_t mDimX = 0;
    int32_t mDimY = 0;
    typedef std::vector<GRoomDesc> GRoomDescs_t;
    GRoomDescs_t mGRoomDescs; // all roomdescs in one vector, with coords to the rooms
    typedef std::deque<glm::vec2> FreeNodes_t;
    GGraph mGraph;
    // returns all the rooms around roomIndex. regardless of cell linkage, or possible link between cells
    NeighbourRooms_t GetPossibleNeighbourRooms( int32_t roomIndex );
    GRoomDesc& GetGRoomDesc( glm::vec2 pos );
    GRoomDesc const& GetGRoomDesc( glm::vec2 pos ) const;
    Cell& GetCell( glm::vec2 pos );
    Cell const& GetCell( glm::vec2 pos ) const;
    glm::vec2 GetCellCoord( glm::vec2 pos ) const;
    RoomDesc& GetRoomDesc( glm::vec2 pos );
    RoomDesc const& GetRoomDesc( glm::vec2 pos ) const;
    void AddCellPair( CellPairs_t& cellPairs, glm::vec2 posA, glm::vec2 posB, int32_t room ) const;
};

} // namespace map

#endif//INCLUDED_MAP_GENERATOR_DATA_H
