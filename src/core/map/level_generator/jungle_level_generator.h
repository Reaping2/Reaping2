#ifndef INCLUDED_MAP_JUNGLE_LEVEL_GENERATOR_H
#define INCLUDED_MAP_JUNGLE_LEVEL_GENERATOR_H

#include "platform/i_platform.h"
#include "i_level_generator.h"
#include "room_repo.h"
#include "room_desc.h"
#include "core/opt.h"

namespace map {


class JungleLevelGenerator: public ILevelGenerator
{
public:
    JungleLevelGenerator( int32_t Id );
    virtual void Generate();
    void CreateStartAndEnd();
    void GenerateTerrain();
    void PlaceRooms();
    Opt<IRoom> PlaceARoom( glm::vec2 &vec );
    void LogNodes( std::string log );
    void GenerateGraph();
private:
    ActorFactory& mActorFactory;
    RoomRepo& mRoomRepo;
    GGraph mGraph;
    typedef std::stack<int32_t> Route_t;
    Route_t mRoute;
    NeighbourRooms_t GetNeighbourRooms( int32_t roomIndex );
    void CreateRoute();
    void LinkRooms();
};

} // namespace map

#endif//INCLUDED_MAP_JUNGLE_LEVEL_GENERATOR_H

