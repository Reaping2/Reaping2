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
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void ITeamComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_TEAM_COMPONENT_H

//command:  "classgenerator" -g "i_component" -c "i_team_component" -m "Team::Type-team"
