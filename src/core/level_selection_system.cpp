#include "platform/i_platform.h"
#include "platform/filesystem.h"
#include "level_selection_system.h"
#include "core/level_selected_event.h"

namespace core {

LevelSelectionSystem::LevelSelectionSystem()
    : mLevelModel( "level", &RootModel::Get() )
    , mSelectLevelModel( StringFunc( this, &LevelSelectionSystem::SelectLevel ), "select", &mLevelModel )
    , mGetSelectedLevelModel( GetStringFunc( this, &LevelSelectionSystem::GetSelectedLevel), "getselected", &mLevelModel ) 
    , mLevelNamesModel( (ModelValue::get_string_vec_t) boost::bind( &LevelSelectionSystem::GetLevelNames, this) , "names", &mLevelModel)
    , mLevelImagesModel( (ModelValue::get_string_vec_t) boost::bind( &LevelSelectionSystem::GetLevelImages, this) , "images", &mLevelModel)
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
        if ( pathit->filename() == "desc.json")
        {
            pathit->remove_filename();
            // store maps by their folder name, but use display name for displaying if not empty
            mLevelNames.push_back(pathit->stem().string());
            L1("%s has desc.json -> adding to list\n", mLevelNames.back().c_str());
        }
        else
        {
            L1("%s misses desc.json\n", pathit->remove_filename().stem().string().c_str() );
        }
    }
}


void LevelSelectionSystem::Update(double DeltaTime)
{
}

void LevelSelectionSystem::SelectLevel( std::string const& Level )
{
    mSelectedLevel = Level;
    L1( "selected level: %s\n", Level.c_str() );
    EventServer<core::LevelSelectedEvent>::Get().SendEvent( core::LevelSelectedEvent( Level ) );
}

std::string LevelSelectionSystem::GetSelectedLevel()
{
    return mSelectedLevel;
} 

std::vector<std::string> LevelSelectionSystem::GetLevelNames()
{
    L1("levelnames ...  size %d\n", mLevelNames.size() );
    for ( auto it = mLevelNames.begin(); it != mLevelNames.end(); ++it )
    {
        L1("level: %s\n", it->c_str() );
    }
    return mLevelNames;
}

std::vector<std::string> LevelSelectionSystem::GetLevelImages()
{
    return mLevelImages;
}

} // namespace core

