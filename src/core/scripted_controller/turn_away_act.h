#ifndef INCLUDED_CORE_SCRIPTED_CONTROLLER_TURN_AWAY_ACT_H
#define INCLUDED_CORE_SCRIPTED_CONTROLLER_TURN_AWAY_ACT_H

#include "i_act.h"
#include "../scene.h"

namespace scriptedcontroller
{

class TurnAwayAct : public IAct
{
public:
    DEFINE_ACT_BASE( TurnAwayAct );
    virtual void Update( Actor& actor, double Seconds );
    virtual void Load( Json::Value const& setters );
    virtual void Start( Actor& actor );
    virtual void Stop( Actor& actor );

    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
private:
    // rotation speed in radians
    double mSpeed = 1.0;
    double mTurn = 0.0;
    double mTurnMax = 1.0;
    double mTurnVariance = 0.0;
    double mTurnCurrent = 0.0;
    Scene& mScene = Scene::Get();
};

template<class Archive>
void TurnAwayAct::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<IAct>( *this );
    ar& mSpeed;
    ar& mTurn;
    ar& mTurnVariance;
    ar& mTurnCurrent;
}


} // namespace scriptedcontroller

#endif//INCLUDED_CORE_SCRIPTED_CONTROLLER_TURN_AWAY_ACT_H
