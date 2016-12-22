#include "platform/i_platform.h"
#include "platform/filesystem.h"
#include "level_selection_system.h"
#include "core/level_selected_event.h"
#include "map/map_repo.h"

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
    // collect available maps:
    // - displayed names
    // - icon.thumbnail
    // - folder name (for loading)
    using namespace map;
    IdStorage & idstorage = IdStorage::Get();
    MapRepo::const_iterator cbegin = MapRepo::Get().begin();
    MapRepo::const_iterator cend = MapRepo::Get().end();
    for ( auto it = cbegin; it != cend; ++it )
    {
        int32_t id = it->first;
        const Json::Value& desc = *(it->second);
        if ( desc.isNull() )
        {
            continue;
        }
        // displayed name
        std::string name;

        std::string foldername;
        idstorage.GetName(id, foldername);
        if ( !Json::GetStr( desc["name"], name) )
        {
            // fallback case: if no name is given, use the containing folder's name
            name = foldername;
        }
        std::string thumbnail;
        Json::GetStr( desc["thumbnail"], thumbnail);

        mLevelRealNames.push_back( foldername );
        mLevelDisplayNames.push_back(name);
        mLevelThumbnails.push_back( AutoId(thumbnail) );
        L1("%s successfully addded to map list as %s\n", mLevelRealNames.back().c_str(), mLevelDisplayNames.back().c_str());
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

