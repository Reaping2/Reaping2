#ifndef INCLUDED_MAP_ROOM_REPO_H
#define INCLUDED_MAP_ROOM_REPO_H

#include "platform/repository.h"
#include "platform/singleton.h"
#include "i_room.h"

namespace map {

class DefaultRoom : public IRoom
{
public:
    DefaultRoom();
    virtual void Generate( RoomDesc& roomDesc, glm::vec2 pos, bool editor = false );
};

class RoomRepo : public platform::Repository<IRoom>, public platform::Singleton<RoomRepo>
{
    friend class platform::Singleton<RoomRepo>;
    static DefaultRoom const mDefault;
    RoomRepo();
    void Init();
public:
    platform::Repository<IRoom>::ElementMap_t const& GetElements();
    void AddRoom( std::unique_ptr<IRoom> iRoom );
};

} // namespace map

#endif//INCLUDED_MAP_ROOM_REPO_H

//command:  "classgenerator.exe" -g "repository" -c "room_repo" -t "i_room" -n "map"
