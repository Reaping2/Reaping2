#include "i_condition.h"
namespace scriptedcontroller
{

ICondition::ICondition( int32_t Id )
    : mId(Id)
{

}


void ICondition::Update( double Seconds )
{

}


bool ICondition::IsSatisfied()
{
    return false;
}

void ICondition::Load( Json::Value const& setters )
{
}

} // namespace scriptedcontroller

