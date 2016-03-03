#include "platform/i_platform.h"
#include "core/pistol.h"
#include "core/audible_event.h"
#include "core/i_move_component.h"

Pistol::Pistol( int32_t Id )
    : Weapon( Id )
{
}

Pistol::Pistol()
    : Weapon( -1 )
{
}

void PistolLoader::BindValues()
{
}

PistolLoader::PistolLoader()
{
    SetBase<WeaponLoader>();
}
