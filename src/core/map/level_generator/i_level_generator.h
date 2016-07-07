#ifndef INCLUDED_MAP_I_LEVEL_GENERATOR_H
#define INCLUDED_MAP_I_LEVEL_GENERATOR_H

#include "platform/i_platform.h"
#include "../../scene.h"
#include "../../actor_factory.h"
#include "room_desc.h"
#include <deque>

namespace map {

// a single cell describing the corresponding room
struct GCell
{
    int32_t mGRoomDescIndex = -1;
    bool mFilled = false; // filled if a room places something into this cell
    glm::vec2 mDescCoord = glm::vec2( -1, -1 ); // relative position inside the room
};

struct GRoomDesc
{
    RoomDesc mRoomDesc;
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
    GGraphNode const& operator[](int32_t nodeIndex) const;
    void Clear();
    void AddNode( GGraphNode const& node );
};

class ILevelGenerator
{
public:

    ILevelGenerator( int32_t Id );
    virtual ~ILevelGenerator();
    virtual void Generate() = 0;
protected:
    typedef std::vector<int32_t> PossibleRooms_t;
    PossibleRooms_t mPossibleRooms;
    typedef std::vector<std::vector<GCell>> GCellMatrix_t;
    GCellMatrix_t mGCells; // all cells with the corresponding roomDesc
    typedef std::vector<GRoomDesc> GRoomDescs_t;
    GRoomDescs_t mGRoomDescs; // all roomdescs in one vector, with coords to the rooms
    typedef std::deque<glm::vec2> FreeNodes_t;
    int32_t mCellSize = 1000;
    int32_t mCellCount = 0;
    int32_t mId = -1;
    Scene& mScene;
    int32_t mStartIndex = -1; // start rooms index at mRoomDescs
    int32_t mEndIndex = -1; // end rooms index at mRoomDescs

    typedef std::pair<glm::vec2, glm::vec2> CellPair_t;
    typedef std::vector<CellPair_t> CellPairs_t;

    void AddPossibleRoom( int32_t roomId, int32_t possibility );
    GCell& GetCell( int32_t x, int32_t y );
    GCell const& GetCell( int32_t x, int32_t y ) const;
    void AddCellPair( CellPairs_t& cellPairs, glm::vec2 posA, glm::vec2 posB, int32_t room );
    bool IsInBounds( glm::vec2 pos ) const;
    bool CanPlaceRoom( RoomDesc const& roomDesc, glm::vec2 pos );
    void PlaceRoom( RoomDesc const& roomDesc, glm::vec2 pos );
    CellPairs_t GetCellPairs( int32_t roomA, int32_t roomB );
};

} // namespace map

#endif//INCLUDED_MAP_I_LEVEL_GENERATOR_H


//command:  "classgenerator.exe" -g "enum" -c "level_generator" -m "int32_t-Id" -n "map"
