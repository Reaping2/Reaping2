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
private:
    ActorFactory& mActorFactory;
    RoomRepo& mRoomRepo;
};

} // namespace map

#endif//INCLUDED_MAP_LEVEL_GENERATOR_H

