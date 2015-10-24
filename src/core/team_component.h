#ifndef INCLUDED_CORE_TEAM_COMPONENT_H
#define INCLUDED_CORE_TEAM_COMPONENT_H

#include "i_team_component.h"
#include "core/property_loader.h"

class TeamComponent : public ITeamComponent
{
public:
    TeamComponent();
    virtual void SetTeam(Team::Type team);
    virtual Team::Type GetTeam()const;
protected:
    friend class ComponentFactory;
    Team::Type mTeam;
private:
};

class TeamComponentLoader : public ComponentLoader<TeamComponent>
{
    virtual void BindValues();
protected:
    TeamComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_TEAM_COMPONENT_H


//command:  "classgenerator" -g "component" -c "team_component" -m "Team::Type-team"
