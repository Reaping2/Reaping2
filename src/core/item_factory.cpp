#include "i_core.h"
#include "core/plasma_gun.h"
#include "core/pistol.h"
#include "core/item.h"
ItemFactory::ItemFactory()
{
    Bind<DefaultItem>( AutoId( "default_item" ) );
    SetDefault( AutoId( "default_item" ) );
    Bind<PlasmaGun>( AutoId( "plasma_gun" ) );
    Bind<Pistol>( AutoId( "pistol" ) );
}
