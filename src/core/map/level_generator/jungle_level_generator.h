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
    virtual void Load( Json::Value const& setters );
private:
    ActorFactory& mActorFactory;
    RoomRepo& mRoomRepo;
    typedef std::stack<int32_t> Route_t;
    typedef std::vector<int32_t> Visited_t;
    Visited_t mVisitedRooms; // a vector of visited room indices
    void LinkRooms( Route_t route );
    struct RouteProperties
    {
        int32_t minLength = 0;
        int32_t endChance = 0;
        int32_t chanceIncrease = 0;
        RoomProperty::Type roomProperty = RoomProperty::Nothing;
        void Load( Json::Value const& setters );
    };
    typedef std::vector<PossibleRooms> MandatoryRooms_t;
    MandatoryRooms_t mMandatoryRooms;
    RouteProperties mMainRouteProperties;
    RouteProperties mSideRouteProperties;
    int32_t mWaypointDistance = 5;
    int32_t mWaypointDistanceVariance = 3;
    Route_t CreateRoute( int32_t startRoomIndex, RouteProperties const& properties );
    FreeNodes_t mFreeCellPositions;
    void CreateMainRoute();
    void CreateSideRoutes();
    int32_t PlaceRoomByProperty( RoomProperty::Type roomProp );
    void GenerateTerrain();
    void CreateWaypoints();
    void RecreateBorders();
    void PlaceRooms();

    void AddNeighboursToFreeCells( IRoom& placedRoom, glm::vec2 const& cellPos );

    Opt<IRoom> PlaceRandomRoom( glm::vec2 const& pos );
    static void LogNodes( std::string log, FreeNodes_t const& nodes );
    void PlaceMandatoryRooms();
    // try to place roomId room. returns roomIndex on success -1 otherwise
    int32_t PlaceRoom( int32_t roomId, PossibleRooms const& possibleRooms );
    void CheckRoomEntrances();
};

} // namespace map

#endif//INCLUDED_MAP_JUNGLE_LEVEL_GENERATOR_H

