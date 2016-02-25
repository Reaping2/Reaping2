#include "core/team_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

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

BOOST_CLASS_EXPORT_IMPLEMENT(TeamComponent);
