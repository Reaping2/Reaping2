#ifndef INCLUDED_CORE_SCRIPTED_CONTROLLER_TURN_TO_TARGET_ACT_H
#define INCLUDED_CORE_SCRIPTED_CONTROLLER_TURN_TO_TARGET_ACT_H

#include "i_act.h"

namespace scriptedcontroller
{

class TurnToTargetAct : public IAct
{
public:
    DEFINE_ACT_BASE( TurnToTargetAct );
    virtual void Update( Actor& actor, double Seconds );
    virtual void Load( Json::Value const& setters );
    virtual void Start( Actor& actor );
    virtual void Stop( Actor& actor );

    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
private:
    // rotation speed in radians
    double mSpeed = 0.0;
    bool mSeekPath = true;
    bool mUseOrientation = true;
};

template<class Archive>
void TurnToTargetAct::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<IAct>( *this );
    ar& mSpeed;
    ar& mSeekPath;
    ar& mUseOrientation;
}


} // namespace scriptedcontroller

#endif//INCLUDED_CORE_SCRIPTED_CONTROLLER_TURN_TO_TARGET_ACT_H
