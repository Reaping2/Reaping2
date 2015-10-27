#include "core/score_on_death_component.h"

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
