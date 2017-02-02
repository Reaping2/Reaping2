#ifndef INCLUDED_CORE_SCRIPTED_CONTROLLER_BITE_ACT_H
#define INCLUDED_CORE_SCRIPTED_CONTROLLER_BITE_ACT_H

#include "i_act.h"
#include "platform/frequency_timer.h"

namespace scriptedcontroller
{

class BiteAct : public IAct
{
public:
    DEFINE_ACT_BASE( BiteAct );
    virtual void Update( Actor& actor, double Seconds );
    virtual void Load( Json::Value const& setters );
    virtual void Start( Actor& actor );
    virtual void Stop( Actor& actor );

    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
private:
    platform::FrequencyTimer mTimer;
    double mDamage = 10.0;
    double mRadius = 50.0;
    bool mCenter = false;
};

template<class Archive>
void BiteAct::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<IAct>( *this );
    ar& mDamage;
    ar& mRadius;
    ar& mCenter;
}


} // namespace scriptedcontroller

#endif//INCLUDED_CORE_SCRIPTED_CONTROLLER_BITE_ACT_H
