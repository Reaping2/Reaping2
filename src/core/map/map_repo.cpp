#include "platform/i_platform.h"
#include "map_repo.h"
#include <memory>

using platform::AutoId;

namespace map {

Json::Value const MapRepo::mDefault = Json::Value(Json::nullValue);

MapRepo::MapRepo()
    : Repository<Json::Value>(mDefault)
{
    Init();
}

void MapRepo::Init()
{
    platform::Filesys& Fs = platform::Filesys::Get();
    // list of available levels/maps
    std::vector<boost::filesystem::path> paths;
    Fs.GetFileNames(paths, "map");
    for ( auto path : paths )
    {
        if ( path.filename() == "description.json")
        {
            AutoFile JsonFile = Fs.Open( path );
            if ( !JsonFile.get() )
            {
                L1("cannot open %s file\n", path.string().c_str() );
                continue;
            }
            JsonReader Reader( *JsonFile );
            if ( !Reader.IsValid() )
            {
                L1("%s is not a valid JSON file", path.filename().string().c_str());
                continue;
            }
            const Json::Value& Root = Reader.GetRoot();
            if ( !Root.isObject() )
            {
                continue;
            }
            path.remove_filename();
            std::string foldername = path.stem().string();

            // index map description with real map name/foldername
            int32_t id = AutoId( foldername );
            mElements.insert(id, new Json::Value(Root));
        }
    }

}

} // namespace map

