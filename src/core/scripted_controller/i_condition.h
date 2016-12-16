#ifndef INCLUDED_CORE_SCRIPTED_CONTROLLER_I_CONDITION_H
#define INCLUDED_CORE_SCRIPTED_CONTROLLER_I_CONDITION_H

#include "core/controller_component.h"
#include "core/property_loader.h"
#include "platform/export.h"
#include <vector>
#include "core/actor.h"

namespace scriptedcontroller
{

// currently there is no need to call any base class method when a method is overridden.

// a condition should determine if something happened on the field with the given actor.
// it's lifetime is Reset on every state change. In other words Reset is called when the event is handled and somewhere all the conditions were satisfied.
// only reset IsSatisfied in Update if that's the intention. (e.g. measured actions like "has_not_been_hit for 4 seconds")
// it's good if an event-ish condition stays satisfied until Reset is called explicitly. 
//   (e.g. "has_been_hit" should be true once it becomes true. Most of the times this condition is true until not handled)
// the given actor is always the same, conditions are instantiated per actor per state per transition
class ICondition
{
public:
    ICondition( int32_t Id );
    ICondition() = default;
    virtual ~ICondition() = default;
    // called every cycle by active transition.
    virtual void Update( Actor& actor, double Seconds );
    // should return if a condition is satisfied. It's a good thing to remain satisfied until reset is called. Although it's not mandatory
    virtual bool IsSatisfied() const;
    // called when a state change occurs. Should reset every member.
    virtual void Reset( Actor& actor );

    virtual void Load( Json::Value const& setters );

    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
protected:
    int32_t mId = -1;
};

template<class Archive>
void ICondition::serialize( Archive& ar, const unsigned int version )
{
    ar & mId;
}

class DefaultCondition : public ICondition
{
public:
    using ICondition::ICondition;
};

} // namespace scriptedcontroller

#endif//INCLUDED_CORE_SCRIPTED_CONTROLLER_I_CONDITION_H
