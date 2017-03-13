#include "platform/i_platform.h"
#include "room_repo.h"
#include "simple_room1.h"
#include "vdouble_room1.h"
#include "hdouble_room1.h"
#include "json_room.h"
#include "platform/filesystem_utils.h"

using platform::AutoId;

namespace map {

DefaultRoom const RoomRepo::mDefault = DefaultRoom();

RoomRepo::RoomRepo()
    : Repository<IRoom>(mDefault)
{
    int32_t id = AutoId( "simple_room1" );
    mElements.insert( id, new SimpleRoom1( id ) );
    id = AutoId( "vdouble_room1" ); mElements.insert( id, new VDoubleRoom1( id ) );
    id = AutoId( "hdouble_room1" ); mElements.insert( id, new HDoubleRoom1( id ) );
    Init();
}

void RoomRepo::Init()
{
    fs_utils::for_each( "rooms", ".json", [&]( Json::Value const& desc )
    {
        std::string str;
        if (Json::GetStr( desc["name"], str ))
        {
            LL() << "Room name:" << str;
            int32_t id = AutoId( str );
            std::unique_ptr<JsonRoom> jsonRoom( new JsonRoom( id ) );
            jsonRoom->Load( desc );
            LL() << "Room load end";
            mElements.insert( id, jsonRoom.release() );
        }
    } );

    for (auto&& e : mElements)
    {
        e.second->Init();
    }
}

platform::Repository<IRoom>::ElementMap_t const& RoomRepo::GetElements()
{
    return mElements;
}

void RoomRepo::AddRoom( std::unique_ptr<IRoom> iRoom )
{
    int32_t id = iRoom->GetId();
    mElements.insert( id, iRoom.release() );
}

DefaultRoom::DefaultRoom()
    : IRoom( -1 )
{
}

void DefaultRoom::Generate( RoomDesc& roomDesc, glm::vec2 pos, bool editor /*= false*/ )
{

}

} // namespace map

