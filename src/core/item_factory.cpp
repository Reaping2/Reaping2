#include "platform/i_platform.h"
#include "core/rocket_launcher.h"
#include "core/plasma_gun.h"
#include "core/pistol.h"
#include "core/item.h"
#include "grenade_normal_item.h"
ItemFactory::ItemFactory()
{
    Bind<DefaultItem>( AutoId( "default_item" ) );
    SetDefault( AutoId( "default_item" ) );
    Bind<PlasmaGun>( AutoId( "plasma_gun" ) );
    Bind<Pistol>( AutoId( "pistol" ) );
	Bind<RocketLauncher>(AutoId("rocket_launcher"));

    Bind<GrenadeNormalItem>(AutoId("grenade_normal_item"));
}
