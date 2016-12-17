#include "i_condition.h"
namespace scriptedcontroller
{

ICondition::ICondition( int32_t Id )
    : mId(Id)
{

}


void ICondition::Update( Actor& actor, double Seconds )
{

}


bool ICondition::IsSatisfied() const
{
    return false;
}

void ICondition::Reset( Actor& actor )
{

}

void ICondition::Load( Json::Value const& setters )
{
}


ICondition* ICondition::clone() const
{
    return new ICondition( *this );
}

} // namespace scriptedcontroller

