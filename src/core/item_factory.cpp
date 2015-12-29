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
ItemFactory::ItemFactory()
{
    Bind<DefaultItem>( AutoId( "default_item" ) );
    SetDefault( AutoId( "default_item" ) );
    Bind<PlasmaGun>( AutoId( "plasma_gun" ) );
    Bind<Pistol>( AutoId( "pistol" ) );
    Bind<Shotgun>( AutoId( "shotgun" ) );
    Bind<RocketLauncher>(AutoId("rocket_launcher"));
    Bind<IonGun>(AutoId("ion_gun"));

    Bind<GrenadeNormalItem>(AutoId("grenade_normal_item"));
    Bind<FlashNormalItem>(AutoId("flash_normal_item"));
    Bind<CloakNormalItem>(AutoId("cloak_normal_item"));

}
