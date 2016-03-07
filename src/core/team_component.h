#ifndef INCLUDED_CORE_TEAM_COMPONENT_H
#define INCLUDED_CORE_TEAM_COMPONENT_H

#include "i_team_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class TeamComponent : public ITeamComponent
{
public:
    TeamComponent();
    virtual void SetTeam( Team::Type team );
    virtual Team::Type GetTeam()const;
protected:
    friend class ComponentFactory;
    Team::Type mTeam;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void TeamComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<ITeamComponent>( *this );
    ar& mTeam;
}

class TeamComponentLoader : public ComponentLoader<TeamComponent>
{
    virtual void BindValues();
protected:
    TeamComponentLoader();
    friend class ComponentLoaderFactory;
};


REAPING2_CLASS_EXPORT_KEY2( TeamComponent, TeamComponent, "team_component" );
#endif//INCLUDED_CORE_TEAM_COMPONENT_H


//command:  "classgenerator" -g "component" -c "team_component" -m "Team::Type-team"
