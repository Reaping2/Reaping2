#include "platform/i_platform.h"
#include "platform/filesystem.h"
#include "level_selection_system.h"
#include "core/level_selected_event.h"

namespace core {

LevelSelectionSystem::LevelSelectionSystem()
    : mLevelModel( RefTo(mSelectedLevel), "level", &RootModel::Get() )
    , mSelectLevelModel( IntFunc( this, &LevelSelectionSystem::SelectLevelByIdx ), "select", &mLevelModel )
    , mLevelDisplayNamesModel( RefTo(mLevelDisplayNames), "names", &mLevelModel)
    , mLevelThumbnailsModel( RefTo(mLevelThumbnails), "images", &mLevelModel)
    , mSelectedLevel( "" )
{
}


void LevelSelectionSystem::Init()
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
            Json::Value Root = Reader.GetRoot();
            if ( !Root.isObject() )
            {
                continue;
            }
            path.remove_filename();
            std::string foldername = path.stem().string();

            std::string desc;
            Json::GetStr( Root["description"], desc );
            std::string name;
            if ( !Json::GetStr( Root["name"], name) )
            {
                // fallback case: if no name is given, use the containing folder's name
                name = foldername;
            }
            std::string thumbnail;
            Json::GetStr( Root["thumbnail"], thumbnail);

            mLevelRealNames.push_back( foldername );
            mLevelDisplayNames.push_back(name);
            mLevelThumbnails.push_back( AutoId(thumbnail) );
            L1("%s successfully addded to map list as %s\n", mLevelRealNames.back().c_str(), mLevelDisplayNames.back().c_str());
        }
        else
        {
            L1("%s misses description.json, so not treating as valid map\n", path.remove_filename().stem().string().c_str() );
        }
    }
}


void LevelSelectionSystem::Update(double DeltaTime)
{
}

void LevelSelectionSystem::SelectLevelByIdx( int32_t idx )
{
    mSelectedLevel = mLevelRealNames[idx];
    L1( "selected level: %s\n", mSelectedLevel.c_str() );
    EventServer<core::LevelSelectedEvent>::Get().SendEvent( core::LevelSelectedEvent( mSelectedLevel ) );
}

void LevelSelectionSystem::SelectLevelByName( std::string realName )
{
    if ( std::find(mLevelRealNames.begin(), mLevelRealNames.end(), realName) == mLevelRealNames.end() )
    {
        L1("attempted selection of invalid level: %d\n", realName.c_str() );
        return;
    }
    mSelectedLevel = realName;
}

std::string LevelSelectionSystem::GetSelectedLevel()
{
    return mSelectedLevel;
}

} // namespace core

