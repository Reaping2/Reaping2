#include "platform/i_platform.h"
#include "room_repo.h"
#include "simple_room1.h"
#include "vdouble_room1.h"
#include "hdouble_room1.h"
#include "json_room.h"

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
    PathVect_t Paths;
    Filesys& FSys = Filesys::Get();
    FSys.GetFileNames( Paths, "rooms" );
    for (PathVect_t::const_iterator i = Paths.begin(), e = Paths.end(); i != e; ++i)
    {
        
        boost::filesystem::path const& Path = *i;
        if (Path.extension().string() != ".json")
        {
            continue;
        }
        AutoFile JsonFile = FSys.Open( *i );
        if (!JsonFile.get())
        {
            continue;
        }
        JsonReader Reader( *JsonFile );
        if (!Reader.IsValid())
        {
            continue;
        }
        Json::Value Root = Reader.GetRoot();
        if (!Root.isArray())
        {
            continue;
        }
        for (auto& jsonRoomDesc : Root )
        {
            std::string str;
            if (Json::GetStr( jsonRoomDesc["name"], str ))
            {
                int32_t id = AutoId( str );
                std::unique_ptr<JsonRoom> jsonRoom( new JsonRoom( id ) );
                jsonRoom->Load( jsonRoomDesc );
                mElements.insert( id, jsonRoom.release() );
            }
        }
    }


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

