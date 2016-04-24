#include "core/rusty_reaper.h"

RustyReaper::RustyReaper( int32_t Id )
    : Weapon( Id )
    , mSawGUID( -1 )
    , mSawActive( false )
{
}

RustyReaper::RustyReaper()
    : Weapon( -1 )
    , mSawGUID( -1 )
    , mSawActive( false )
{
}

void RustyReaper::SetSawGUID( int32_t sawGUID )
{
    mSawGUID = sawGUID;
}

int32_t RustyReaper::GetSawGUID()const
{
    return mSawGUID;
}

void RustyReaper::SetSawActive( bool sawActive )
{
    mSawActive = sawActive;
}

bool RustyReaper::IsSawActive()const
{
    return mSawActive;
}


void RustyReaperLoader::BindValues()
{
}

RustyReaperLoader::RustyReaperLoader()
{
    SetBase<WeaponLoader>();
}


REAPING2_CLASS_EXPORT_IMPLEMENT( RustyReaper, RustyReaper );
