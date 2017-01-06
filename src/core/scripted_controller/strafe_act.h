#ifndef INCLUDED_CORE_SCRIPTED_CONTROLLER_MOVE_STRAFE_ACT_H
#define INCLUDED_CORE_SCRIPTED_CONTROLLER_MOVE_STRAFE_ACT_H

#include "i_act.h"

namespace scriptedcontroller
{

class StrafeAct : public IAct
{
public:
    DEFINE_ACT_BASE( StrafeAct );
    virtual void Update( Actor& actor, double Seconds );
    virtual void Load( Json::Value const& setters );
    virtual void Start( Actor& actor );
    virtual void Stop( Actor& actor );

    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
private:
    double mSpeed = 100.0;

    double mHeadingRotation = 0.0;
};

template<class Archive>
void StrafeAct::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<IAct>( *this );
    ar& mSpeed;
    ar& mHeadingRotation;
}


} // namespace scriptedcontroller

#endif//INCLUDED_CORE_SCRIPTED_CONTROLLER_MOVE_STRAFE_ACT_H
