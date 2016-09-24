#ifndef INCLUDED_MAP_I_LEVEL_GENERATOR_H
#define INCLUDED_MAP_I_LEVEL_GENERATOR_H

#include "platform/i_platform.h"
#include "../../scene.h"
#include "../../actor_factory.h"
#include "room_desc.h"
#include <deque>
#include "generator_data.h"

namespace map {

class ILevelGenerator
{
public:

    ILevelGenerator( int32_t Id );
    virtual ~ILevelGenerator();
    virtual void Generate() = 0;
    virtual void Load( Json::Value& setters ); 
protected:
    typedef std::vector<int32_t> PossibleRooms_t;
    PossibleRooms_t mPossibleRoomIds;
    typedef std::deque<glm::vec2> FreeNodes_t;
    int32_t mCellSize = 1000;
    int32_t mCellCount = 5;
    int32_t mId = -1;
    Scene& mScene;
    int32_t mStartRoomIndex = -1; // start rooms index at mRoomDescs
    int32_t mEndRoomIndex = -1; // end rooms index at mRoomDescs
    GeneratorData mGData;
    uint32_t mSeed = 0;
    void AddPossibleRoom( int32_t roomId, int32_t possibility );
};

class DefaultLevelGenerator : public ILevelGenerator
{
public:
    DefaultLevelGenerator( int32_t Id );
    virtual void Generate();
};


} // namespace map

#endif//INCLUDED_MAP_I_LEVEL_GENERATOR_H


//command:  "classgenerator.exe" -g "enum" -c "level_generator" -m "int32_t-Id" -n "map"
