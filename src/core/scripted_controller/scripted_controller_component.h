#ifndef INCLUDED_CORE_SCRIPTED_CONTROLLER_COMPONENT_H
#define INCLUDED_CORE_SCRIPTED_CONTROLLER_COMPONENT_H

#include "core/controller_component.h"
#include "core/property_loader.h"
#include "platform/export.h"
#include <vector>
#include "state.h"
#include <boost/serialization/map.hpp>
namespace scriptedcontroller {

class ScriptedControllerComponent : public ControllerComponent
{
public:

    ScriptedControllerComponent();
    typedef std::map<int32_t, State> States_t;
    virtual void SetStates( States_t states );
    virtual States_t& GetStates();
    virtual void SetStateIdentifier( int32_t StateIdentifier );
    virtual int32_t GetStateIdentifier() const;
    virtual Opt<State> GetState();
protected:
    States_t mStates;
    int32_t mStateIdentifier;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ScriptedControllerComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<ControllerComponent>(*this);
    ar& mStates;
    ar& mStateIdentifier;
}

class ScriptedControllerComponentLoader : public ComponentLoader<ScriptedControllerComponent>
{
public:
    DEFINE_COMPONENT_LOADER_BASE( ScriptedControllerComponentLoader )
    ScriptedControllerComponentLoader();
    virtual void BindValues();
};
} // namespace scriptedcontroller
REAPING2_CLASS_EXPORT_KEY2( scriptedcontroller__ScriptedControllerComponent, scriptedcontroller::ScriptedControllerComponent, "scripted_controller_component" );


#endif//INCLUDED_CORE_SCRIPTED_CONTROLLER_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -p "controller_component" -c "scripted_controller_component" -m "States_t-states"
