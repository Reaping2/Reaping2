#ifndef INCLUDED_CORE_SCRIPTED_CONTROLLER_DISTANCE_TO_AGGRESSOR_CONDITION_H
#define INCLUDED_CORE_SCRIPTED_CONTROLLER_DISTANCE_TO_AGGRESSOR_CONDITION_H

#include "i_condition.h"
#include "..\..\platform\frequency_timer.h"

namespace scriptedcontroller
{

class DistanceToAggressorCondition : public ICondition
{
public:
    DEFINE_CONDITION_BASE( DistanceToAggressorCondition )
    virtual void Update( Actor& actor, double Seconds );
    virtual bool IsSatisfied() const;
    virtual void Reset( Actor& actor );
    virtual void Load( Json::Value const& setters );

    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
protected:
    int32_t mDistance = std::numeric_limits<int32_t>::max();
    int32_t mLessThan = std::numeric_limits<int32_t>::max();
    int32_t mGreaterThan = std::numeric_limits<int32_t>::min();
    FrequencyTimer mTimer;
};

template<class Archive>
void DistanceToAggressorCondition::serialize( Archive& ar, const unsigned int version )
{
    ar & boost::serialization::base_object<ICondition>( *this );
    ar & mDistance;
    ar & mLessThan;
    ar & mGreaterThan;
}

} // namespace scriptedcontroller

#endif//INCLUDED_CORE_SCRIPTED_CONTROLLER_DISTANCE_TO_AGGRESSOR_CONDITION_H
