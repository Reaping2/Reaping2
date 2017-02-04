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
    Fs.GetFileNames(paths, mMapDir );
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
            Json::Value& Root = Reader.GetRoot();
            if ( !Root.isObject() )
            {
                continue;
            }
            path.remove_filename();
            std::string foldername = path.stem().string();
            // query 'generated' info, this distinguishes regular maps from rogue maps
            // info is available in map_elements.json
            {
                // default value
                Root["generated"] = false;
                AutoFile MapElementsFile = Fs.Open( mMapDir +"/"+foldername+"/map_elements.json");
                if (!MapElementsFile.get())
                {
                    L1("Cannot open file %s/map_elements.json", foldername.c_str());
                    continue;
                }
                JsonReader MapElementsReader( *MapElementsFile );
                if (!MapElementsReader.IsValid())
                {
                    L1("%s/map_elements.json is not a valid JSON file", foldername.c_str());
                    continue;
                }
                const Json::Value& MapElementsRoot = MapElementsReader.GetRoot();
                if (MapElementsRoot.isArray())
                {
                    for ( auto const& item: MapElementsRoot )
                    {
                        std::string name;
                        if (Json::GetStr(item["name"], name))
                        {
                            if ("level_generated" == name)
                            {
                                Root["generated"] = true;
                                break;
                            }
                        }
                    }
                }
            }

            // index map description with real map name/foldername
            int32_t id = AutoId( foldername );
            mElements.insert(id, new Json::Value(Root));
        }
    }
}

std::string const MapRepo::mMapDir = "maps";

} // namespace map

