#ifndef INCLUDED_CORE_SCRIPTED_CONTROLLER_RANDOM_CONDITION_H
#define INCLUDED_CORE_SCRIPTED_CONTROLLER_RANDOM_CONDITION_H

#include "i_condition.h"
#include "platform/frequency_timer.h"

namespace scriptedcontroller
{

class RandomCondition : public ICondition
{
public:
    DEFINE_CONDITION_BASE( RandomCondition )
    virtual void Update( Actor& actor, double Seconds );
    virtual bool IsSatisfied() const;
    virtual void Reset( Actor& actor );
    virtual void Load( Json::Value const& setters );

    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
protected:
    double mPercent = 0.0;
    FrequencyTimer mTimer;
    bool mSatisfied = false;
};

template<class Archive>
void RandomCondition::serialize( Archive& ar, const unsigned int version )
{
    ar & boost::serialization::base_object<ICondition>( *this );
    ar & mPercent;
}

} // namespace scriptedcontroller

#endif//INCLUDED_CORE_SCRIPTED_CONTROLLER_RANDOM_CONDITION_H
