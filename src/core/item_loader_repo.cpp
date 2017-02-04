#include "platform/i_platform.h"
#include "item_loader_repo.h"
#include "property_loader.h"
#include "item.h"
#include "plasma_gun.h"
#include "pistol.h"
#include "shotgun.h"
#include "rocket_launcher.h"
#include "ion_gun.h"
#include "gatling_gun.h"
#include "gauss_gun.h"
#include "lucky_rocket.h"
#include "rusty_reaper.h"

using platform::AutoId;

DefaultItemLoader const ItemLoaderRepo::mDefault;
ItemLoaderRepo::ItemLoaderRepo()
    : Repository<PropertyLoaderBase<Item> >( mDefault )
{
    int32_t id = AutoId( "plasma_gun" );
    mElements.insert( id, new PlasmaGunLoader() );
    id = AutoId( "guard_plasma_gun" );
    mElements.insert( id, new PlasmaGunLoader() );
    id = AutoId( "pistol" );
    mElements.insert( id, new PistolLoader() );
    id = AutoId( "shotgun" );
    mElements.insert( id, new ShotgunLoader() );
    id = AutoId( "rocket_launcher" );
    mElements.insert( id, new RocketLauncherLoader() );
    id = AutoId( "ion_gun" );
    mElements.insert( id, new IonGunLoader() );
    id = AutoId( "gatling_gun" );
    mElements.insert( id, new GatlingGunLoader() );
    id = AutoId( "gauss_gun" );
    mElements.insert( id, new GaussGunLoader() );
    id = AutoId( "lucky_rocket" );
    mElements.insert( id, new LuckyRocketLoader() );
    id = AutoId( "rusty_reaper"); 
    mElements.insert(id, new RustyReaperLoader());
    Init();
}

void ItemLoaderRepo::Init()
{
    PathVect_t Paths;
    Filesys& FSys = Filesys::Get();
    FSys.GetFileNames( Paths, "actors" );
    for( PathVect_t::const_iterator i = Paths.begin(), e = Paths.end(); i != e; ++i )
    {
        boost::filesystem::path const& Path = *i;
        if( Path.extension().string() != ".item" )
        {
            continue;
        }
        AutoFile JsonFile = FSys.Open( *i );
        if( !JsonFile.get() )
        {
            continue;
        }
        JsonReader Reader( *JsonFile );
        if( !Reader.IsValid() )
        {
            continue;
        }
        Json::Value Root = Reader.GetRoot();
        if( !Root.isArray() )
        {
            continue;
        }
        for( Json::Value::iterator i = Root.begin(), e = Root.end(); i != e; ++i )
        {
            Json::Value& ItemDesc = *i;
            if( !LoadItemFromOneDesc( ItemDesc ) )
            {
                return;
            }
        }
    }

}

bool ItemLoaderRepo::LoadItemFromOneDesc( Json::Value& ItemDesc )
{
    std::string nameStr;
    if( !Json::GetStr( ItemDesc["name"], nameStr ) )
    {
        return false;
    }
    PropertyLoaderBase<Item>& itemLoader = operator()( AutoId( nameStr ) );

    Json::Value& setters = ItemDesc["set"];
    if ( !setters.isArray() )
    {
        return false;
    }
    if ( setters.empty() )
    {
        return true;
    }
    L1( "Load item_loader: %s\n", nameStr.c_str() );
    itemLoader.Load( *setters.begin() );
    return true;
}

