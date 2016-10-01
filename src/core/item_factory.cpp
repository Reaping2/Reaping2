#include "platform/i_platform.h"
#include "core/rocket_launcher.h"
#include "core/plasma_gun.h"
#include "core/pistol.h"
#include "core/item.h"
#include "grenade_normal_item.h"
#include "flash_normal_item.h"
#include "shotgun.h"
#include "ion_gun.h"
#include "cloak_normal_item.h"
#include "gatling_gun.h"
#include "gauss_gun.h"
#include "lucky_rocket.h"
#include "blue_grenade_normal_item.h"
#include "rusty_reaper.h"

ItemFactory::ItemFactory()
{
    Bind<DefaultItem>( AutoId( "default_item" ) );
    SetDefault( AutoId( "default_item" ) );
    Bind( AutoId( "plasma_gun" ), &CreateItem<PlasmaGun> );
    Bind( AutoId( "guard_plasma_gun" ), &CreateItem<PlasmaGun> );
    Bind( AutoId( "pistol" ), &CreateItem<Pistol> );
    Bind( AutoId( "shotgun" ), &CreateItem<Shotgun> );
    Bind( AutoId( "rocket_launcher" ), &CreateItem<RocketLauncher> );
    Bind( AutoId( "ion_gun" ), &CreateItem<IonGun> );
    Bind( AutoId( "gatling_gun" ), &CreateItem<GatlingGun> );
    Bind( AutoId( "gauss_gun" ), &CreateItem<GaussGun> );
    Bind( AutoId( "lucky_rocket" ), &CreateItem<LuckyRocket> );
    Bind( AutoId( "rusty_reaper" ), &CreateItem<RustyReaper>);

    Bind<GrenadeNormalItem>( AutoId( "grenade_normal_item" ) );
    Bind<FlashNormalItem>( AutoId( "flash_normal_item" ) );
    Bind<CloakNormalItem>( AutoId( "cloak_normal_item" ) );
    Bind<BlueGrenadeNormalItem>( AutoId( "blue_grenade_normal_item" ) );
}
