#include "core/kill_score_on_death_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

KillScoreOnDeathComponent::KillScoreOnDeathComponent()
    : mKillerGUID(-1)
    , mDeadGUID(-1)
    , mScored(false)
{
}

void KillScoreOnDeathComponent::SetKillerGUID(int32_t killerGUID)
{
    mKillerGUID=killerGUID;
}

int32_t KillScoreOnDeathComponent::GetKillerGUID()const
{
    return mKillerGUID;
}

void KillScoreOnDeathComponent::SetDeadGUID(int32_t deadGUID)
{
    mDeadGUID=deadGUID;
}

int32_t KillScoreOnDeathComponent::GetDeadGUID()const
{
    return mDeadGUID;
}

void KillScoreOnDeathComponent::SetScored(bool scored)
{
    mScored=scored;
}

bool KillScoreOnDeathComponent::IsScored()const
{
    return mScored;
}



void KillScoreOnDeathComponentLoader::BindValues()
{
}

KillScoreOnDeathComponentLoader::KillScoreOnDeathComponentLoader()
{
}

BOOST_CLASS_EXPORT_IMPLEMENT(KillScoreOnDeathComponent);
