#ifndef INCLUDED_MAP_I_LEVEL_GENERATOR_H
#define INCLUDED_MAP_I_LEVEL_GENERATOR_H

#include "platform/i_platform.h"
#include "../../scene.h"
#include "../../actor_factory.h"
#include "room_desc.h"
#include <deque>

namespace map {

// a single cell describing the corresponding room
struct GeneratorCell
{
    RoomDesc mRoomDesc;
    bool mFilled = false; // filled if a room places something into this cell
    glm::vec2 mDescCoord = glm::vec2( -1, -1 ); // relative position inside the room
    glm::vec2 mRoomCoord = glm::vec2( -1, -1 ); // absolute position of the corresponding rooms bottom left
};

struct GeneratorRoomDesc
{
    RoomDesc mRoomDesc;
    glm::vec2 mRoomCoord = glm::vec2( -1, -1 );
};

struct RoomNode
{
    Opt<IRoom> mRoot;
    std::vector<RoomNode> mNodes;
};

class ILevelGenerator
{
public:
    typedef std::vector<int32_t> PossibleRooms_t;
    PossibleRooms_t mPossibleRooms;
    typedef std::vector<std::vector<GeneratorCell>> CellMatrix_t;
    CellMatrix_t mCells; // all cells with the corresponding roomDesc
    typedef std::vector<GeneratorRoomDesc> RoomDescs_t;
    RoomDescs_t mRoomDescs; // all roomdesc in one vector, with coords to the rooms
    typedef std::deque<glm::vec2> Neighbours_t;
    Neighbours_t mFreeNodes;
    int32_t mCellSize = 1000;
    int32_t mCellCount = 0;
    ILevelGenerator( int32_t Id );
    virtual ~ILevelGenerator();
    virtual void Generate() = 0;
    bool IsInBounds( int32_t x, int32_t y ) const;
protected:
    int32_t mId = -1;
    Scene& mScene;
    int32_t mStartIndex = -1; // start rooms index at mRoomDescs
    int32_t mEndIndex = -1; // end rooms index at mRoomDescs
    void AddPossibleRoom( int32_t roomId, int32_t possibility );
};

} // namespace map

#endif//INCLUDED_MAP_I_LEVEL_GENERATOR_H


//command:  "classgenerator.exe" -g "enum" -c "level_generator" -m "int32_t-Id" -n "map"
