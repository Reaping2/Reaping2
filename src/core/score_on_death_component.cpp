#include "core/score_on_death_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

ScoreOnDeathComponent::ScoreOnDeathComponent()
    : mScored(false)
{
}

void ScoreOnDeathComponent::SetScored(bool scored)
{
    mScored=scored;
}

bool ScoreOnDeathComponent::IsScored()const
{
    return mScored;
}



void ScoreOnDeathComponentLoader::BindValues()
{
}

ScoreOnDeathComponentLoader::ScoreOnDeathComponentLoader()
{
}

BOOST_CLASS_EXPORT_IMPLEMENT(ScoreOnDeathComponent);
