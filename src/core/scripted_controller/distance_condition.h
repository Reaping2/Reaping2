#ifndef INCLUDED_CORE_SCRIPTED_CONTROLLER_DISTANCE_CONDITION_H
#define INCLUDED_CORE_SCRIPTED_CONTROLLER_DISTANCE_CONDITION_H

#include "i_condition.h"
#include "platform/frequency_timer.h"

namespace scriptedcontroller
{

#define DEFINE_DISTANCE_CONDITION_BASE( ConditionType ) \
    using DistanceCondition::DistanceCondition; \
    virtual ICondition* clone() const \
    { \
        return new ConditionType( *this ); \
    } \

class DistanceCondition : public ICondition
{
public:
    DEFINE_CONDITION_BASE( DistanceCondition )
    virtual void Update( Actor& actor, double Seconds );
    virtual bool IsSatisfied() const;
    virtual void Reset( Actor& actor );
    virtual void Load( Json::Value const& setters );

    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
protected:
    double mDistance = std::numeric_limits<double>::max();
    double mLessThan = std::numeric_limits<double>::max();
    double mGreaterThan = std::numeric_limits<double>::min();
    bool mSeekPath = true;
};

template<class Archive>
void DistanceCondition::serialize( Archive& ar, const unsigned int version )
{
    ar & boost::serialization::base_object<ICondition>( *this );
    ar & mDistance;
    ar & mLessThan;
    ar & mGreaterThan;
    ar & mSeekPath;
}

} // namespace scriptedcontroller

namespace core
{
    double GetDistance( Actor& actor, Actor& target, bool seekPath = true );
}
#endif//INCLUDED_CORE_SCRIPTED_CONTROLLER_DISTANCE_CONDITION_H
