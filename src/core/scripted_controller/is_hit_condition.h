#ifndef INCLUDED_CORE_SCRIPTED_CONTROLLER_IS_HIT_CONDITION_H
#define INCLUDED_CORE_SCRIPTED_CONTROLLER_IS_HIT_CONDITION_H

#include "i_condition.h"

namespace scriptedcontroller
{

class IsHitCondition : public ICondition
{
public:
    DEFINE_CONDITION_BASE( IsHitCondition )
    virtual void Update( Actor& actor, double Seconds );
    virtual bool IsSatisfied() const;
    virtual void Reset( Actor& actor );
    virtual void Load( Json::Value const& setters );

    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
protected:
    bool mIsHit = false;
    double mLastDamageTime = -1.0;
};

template<class Archive>
void IsHitCondition::serialize( Archive& ar, const unsigned int version )
{
    ar & boost::serialization::base_object<ICondition>( *this );
    ar & mIsHit;
    ar & mLastDamageTime;
}

} // namespace scriptedcontroller

#endif//INCLUDED_CORE_SCRIPTED_CONTROLLER_IS_HIT_CONDITION_H
