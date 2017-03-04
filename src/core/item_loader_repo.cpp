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
#include "platform/filesystem_utils.h"

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
    fs_utils::for_each( "actors", ".item", [&]( Json::Value const& desc )
    {
        std::string nameStr;
        if (!Json::GetStr( desc["name"], nameStr ))
        {
            return;
        }
        PropertyLoaderBase<Item>& itemLoader = operator()( AutoId( nameStr ) );

        auto const& setters = desc["set"];
        if (!setters.isArray())
        {
            return;
        }
        if (setters.empty())
        {
            return;
        }
        L2( "Load item_loader: %s\n", nameStr.c_str() );
        itemLoader.Load( *setters.begin() );
    } );

}

