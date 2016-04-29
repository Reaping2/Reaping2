#ifndef INCLUDED_MAP_I_LEVEL_GENERATOR_H
#define INCLUDED_MAP_I_LEVEL_GENERATOR_H

#include "platform/i_platform.h"
#include "../../scene.h"
#include "../../actor_factory.h"
#include "room_desc.h"
#include <deque>

namespace map {

struct GeneratorCell
{
    RoomDesc mRoomDesc;
    bool mFilled = false;
    glm::vec2 mDescCoord = glm::vec2( -1, -1 );
    glm::vec2 mRoomCoord = glm::vec2( -1, -1 );
};

struct GeneratorRoomDesc
{
    RoomDesc mRoomDesc;
    glm::vec2 mRoomCoord = glm::vec2( -1, -1 );
};

class ILevelGenerator
{
public:
    typedef std::vector<std::vector<GeneratorCell>> CellMatrix_t;
    CellMatrix_t mCells;
    typedef std::vector<GeneratorRoomDesc> RoomDescs_t;
    RoomDescs_t mRoomDescs;
    typedef std::deque<glm::vec2> Neighbours_t;
    Neighbours_t mFreeNodes;
    int32_t mCellSize = 1000;
    int32_t mCellCount = 0;
    ILevelGenerator( int32_t Id );
    virtual ~ILevelGenerator();
    virtual void Generate() = 0;
protected:
    int32_t mId = -1;
    Scene& mScene;
};

} // namespace map

#endif//INCLUDED_MAP_I_LEVEL_GENERATOR_H


//command:  "classgenerator.exe" -g "enum" -c "level_generator" -m "int32_t-Id" -n "map"
