#ifndef INCLUDED_CORE_I_TEAM_COMPONENT_H
#define INCLUDED_CORE_I_TEAM_COMPONENT_H

#include "component.h"
#include "ctf_program_state.h"

class ITeamComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(ITeamComponent)
    virtual void SetTeam(Team::Type team)=0;
    virtual Team::Type GetTeam()const=0;
};

#endif//INCLUDED_CORE_I_TEAM_COMPONENT_H

//command:  "classgenerator" -g "i_component" -c "i_team_component" -m "Team::Type-team"
