#include "core/team_component.h"

TeamComponent::TeamComponent()
    : mTeam(Team::Blue)
{
}

void TeamComponent::SetTeam(Team::Type team)
{
    mTeam=team;
}

Team::Type TeamComponent::GetTeam()const
{
    return mTeam;
}



void TeamComponentLoader::BindValues()
{
}

TeamComponentLoader::TeamComponentLoader()
{
}
