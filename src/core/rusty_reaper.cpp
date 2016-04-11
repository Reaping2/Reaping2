#include "core/rusty_reaper.h"

RustyReaper::RustyReaper( int32_t Id )
    : Weapon( Id )
{
}

RustyReaper::RustyReaper()
    : Weapon( -1 )
{
}



void RustyReaperLoader::BindValues()
{
}

RustyReaperLoader::RustyReaperLoader()
{
    SetBase<WeaponLoader>();
}


REAPING2_CLASS_EXPORT_IMPLEMENT( RustyReaper, RustyReaper );
