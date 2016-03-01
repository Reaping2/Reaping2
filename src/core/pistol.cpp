#include "platform/i_platform.h"
#include "core/pistol.h"
#include "core/audible_event.h"
#include "core/i_move_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>


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
REAPING2_CLASS_EXPORT_IMPLEMENT(Pistol, Pistol);
