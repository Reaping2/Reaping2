#ifndef INCLUDED_CORE_SCRIPTED_CONTROLLER_I_ACT_H
#define INCLUDED_CORE_SCRIPTED_CONTROLLER_I_ACT_H

#include "core/controller_component.h"
#include "core/property_loader.h"
#include "platform/export.h"
#include <vector>
#include "core/actor.h"

namespace scriptedcontroller
{

// it's highly advised to call base class methods when a method is overridden.

// an act should do some dirty things while it is active. It's lifetime depends on mDurationCurrent. 
// mDurationCurrent is managed by base class and by the parameters from json.
// an act can have a state, can depend on any attributes of the given actor, or the scene
// the given actor is always the same, acts are instantiated per actor per state
class IAct
{
public:
    IAct( int32_t Id );
    IAct() = default;
    virtual ~IAct() = default;
    // called every cycle when act is active.
    virtual void Update( Actor& actor, double Seconds );
    // "params" passed
    virtual void Load( Json::Value const& setters );
    // called when act is selected and started.
    virtual void Start( Actor& actor );
    // called when act is interrupted or if not interruptible when act finished running
    virtual void Stop( Actor& actor );

    // checks if an act is interruptible. use mInterruptible, override only in S.P.E.C.I.A.L. cases.
    virtual bool IsInterruptible();
    // decides if act finished running. simply checks mDurationCurrent>0. override only in S.P.E.C.I.A.L. cases.
    virtual bool IsRunning();
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
protected:
    int32_t mId = -1;
    double mDuration = 0.0;
    double mDurationVariance = 0.0;
    double mDurationCurrent = 0.0;
    bool mInterruptible = false;
};

template<class Archive>
void IAct::serialize( Archive& ar, const unsigned int version )
{
    ar & mId;
    ar & mDuration;
    ar & mDurationVariance;
    ar & mDurationCurrent;
    ar & mInterruptible;
}

class DefaultAct : public IAct
{
public:
    using IAct::IAct;
};

} // namespace scriptedcontroller

#endif//INCLUDED_CORE_SCRIPTED_CONTROLLER_I_ACT_H
