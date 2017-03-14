#include "platform/i_platform.h"
#include "platform/filesystem.h"
#include "level_selection_system.h"
#include "core/level_selected_event.h"
#include "map/map_repo.h"

namespace core {

LevelSelectionSystem::LevelSelectionSystem()
    : mLevelModel( RefTo(mSelectedLevel), "level", &RootModel::Get() )
    , mSelectLevelModel( IntFunc( this, &LevelSelectionSystem::SelectLevelByIdx ), "select", &mLevelModel )
    , mSelectLevelByNameModel( StringFunc( this, &LevelSelectionSystem::SelectLevelByNameUI ), "select_by_name", &mLevelModel )
    , mLevelDisplayNamesModel( (ModelValue::get_string_vec_t)boost::bind( &LevelSelectionSystem::DisplayNames, this), "names", &mLevelModel )
    , mLevelThumbnailsModel( (ModelValue::get_int_vec_t)boost::bind(&LevelSelectionSystem::Thumbnails, this ), "images", &mLevelModel )
    , mSelectedLevel( "" )
    , mGameMode( GameModes::Unknown )
{
}


void LevelSelectionSystem::Init()
{
    mOnGamemodeSelectedEvent = EventServer<core::GamemodeSelectedEvent>::Get().Subscribe( boost::bind( &LevelSelectionSystem::OnGamemodeSelectedEvent, this, _1 ) );

    // collect available maps:
    // - displayed names
    // - icon/thumbnail
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
        // store map for all the gamemodes it's available for
        std::vector<GameModes::Type> gamemodes;
        if ( desc["maptype"].isArray() )
        {
            for ( const auto& type : desc["maptype"] )
            {
                std::string t;
                Json::GetStr( type, t );
                GameModes::Type m = GameModes::Get()( AutoId(t) );
                gamemodes.push_back(m);
            }
        }

        std::string foldername;
        idstorage.GetName(id, foldername);

        // displayed name
        std::string name;
        if ( !Json::GetStr( desc["name"], name) )
        {
            // fallback case: if no name is given, use the containing folder's name
            name = foldername;
        }
        std::string thumbnail;
        Json::GetStr( desc["thumbnail"], thumbnail);

        for ( const auto& gamemode : gamemodes )
        {
            mLevelRealNames[gamemode].push_back( foldername );
            mLevelDisplayNames[gamemode].push_back(name);
            mLevelThumbnails[gamemode].push_back( AutoId(thumbnail) );
            L2("%s successfully addded to map list as %s\n", mLevelRealNames[gamemode].back().c_str(), mLevelDisplayNames[gamemode].back().c_str());
        }
    }
}


void LevelSelectionSystem::Update(double DeltaTime)
{
}

void LevelSelectionSystem::SelectLevelByIdx( int32_t idx )
{
    SelectLevelByNameUI( mLevelRealNames[mGameMode][idx] );
}

void LevelSelectionSystem::SelectLevelByNameUI( std::string const& levelName )
{
    mSelectedLevel = levelName;
    L1( "selected level: %s\n", mSelectedLevel.c_str() );
    EventServer<core::LevelSelectedEvent>::Get().SendEvent( core::LevelSelectedEvent( mSelectedLevel ) );
}

void LevelSelectionSystem::SelectLevelByName( GameModes::Type gameMode, std::string const& realName )
{
    mGameMode = gameMode;
    std::vector<std::string> const& realNames = mLevelRealNames[mGameMode];
    if ( std::find(realNames.begin(), realNames.end(), realName) == realNames.end() )
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

std::vector<std::string> LevelSelectionSystem::DisplayNames()
{
    return mLevelDisplayNames[mGameMode];
}

std::vector<int32_t> LevelSelectionSystem::Thumbnails()
{
    return mLevelThumbnails[mGameMode];
}

void LevelSelectionSystem::OnGamemodeSelectedEvent( core::GamemodeSelectedEvent const& evt )
{
    mGameMode = evt.mGameMode;
}

} // namespace core

