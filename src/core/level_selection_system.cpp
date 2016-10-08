#include "platform/i_platform.h"
#include "platform/filesystem.h"
#include "level_selection_system.h"
#include "core/level_selected_event.h"

namespace core {

LevelSelectionSystem::LevelSelectionSystem()
    : mLevelModel( "level", &RootModel::Get() )
    , mSelectLevelModel( IntFunc( this, &LevelSelectionSystem::SelectLevel ), "select", &mLevelModel )
    , mGetSelectedLevelModel( GetStringFunc( this, &LevelSelectionSystem::GetSelectedLevel), "getselected", &mLevelModel ) 
    , mLevelDisplayNamesModel( (ModelValue::get_string_vec_t) boost::bind( &LevelSelectionSystem::GetLevelDisplayNames, this) , "names", &mLevelModel)
    , mLevelThumbnailsModel( (ModelValue::get_int_vec_t) boost::bind( &LevelSelectionSystem::GetLevelThumbnails, this) , "images", &mLevelModel)
    , mSelectedLevel( "" )
{
}


void LevelSelectionSystem::Init()
{
    platform::Filesys& Fs = platform::Filesys::Get();
    // list of available levels/maps
    std::vector<boost::filesystem::path> paths;
    Fs.GetFileNames(paths, "map");
    for ( auto pathit = paths.begin(); pathit != paths.end(); ++pathit )
    {
        if ( pathit->filename() == "description.json")
        {
            AutoFile JsonFile = Fs.Open( *pathit );
            if ( !JsonFile.get() )
            {
                L1("cannot open %s file\n", pathit->string().c_str() );
                continue;
            }
            JsonReader Reader( *JsonFile );
            if ( !Reader.IsValid() )
            {
                L1("%s is not a valid JSON file", pathit->filename().string().c_str());
                continue;
            }
            Json::Value Root = Reader.GetRoot();
            if ( !Root.isObject() )
            {
                continue;
            }
            pathit->remove_filename();
            std::string foldername = pathit->stem().string();

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
            L1("%s misses description.json, so not treating as valid map\n", pathit->remove_filename().stem().string().c_str() );
        }
    }
}


void LevelSelectionSystem::Update(double DeltaTime)
{
}

void LevelSelectionSystem::SelectLevel( int32_t idx )
{
    mSelectedLevel = mLevelRealNames[idx];
    L1( "selected level: %s\n", mSelectedLevel.c_str() );
    EventServer<core::LevelSelectedEvent>::Get().SendEvent( core::LevelSelectedEvent( mSelectedLevel ) );
}

std::string LevelSelectionSystem::GetSelectedLevel()
{
    return mSelectedLevel;
} 

std::vector<std::string> LevelSelectionSystem::GetLevelDisplayNames()
{
    return mLevelDisplayNames;
}

std::vector<int32_t> LevelSelectionSystem::GetLevelThumbnails()
{
    return mLevelThumbnails;
}

} // namespace core

