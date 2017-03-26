#include "core/attractable_component.h"

AttractableComponent::AttractableComponent()
    : mType(-1)
    , mDeceleration(500.0)
    , mTurnToTargetAct()
{
}

void AttractableComponent::SetAttractType( int32_t type )
{
    mType = type;
}

int32_t AttractableComponent::GetAttractType() const
{
    return mType;
}

void AttractableComponent::SetDeceleration( int32_t deceleration )
{
    mDeceleration = deceleration;
}

int32_t AttractableComponent::GetDeceleration() const
{
    return mDeceleration;
}

void AttractableComponent::SetTurnToTargetAct( scriptedcontroller::TurnToTargetAct turnToTargetAct )
{
    mTurnToTargetAct = turnToTargetAct;
}

scriptedcontroller::TurnToTargetAct& AttractableComponent::GetTurnToTargetAct()
{
    return mTurnToTargetAct;
}



void AttractableComponentLoader::BindValues()
{
    std::string istr;
    if (Json::GetStr( (*mSetters)["type"], istr ))
    {
        int32_t typ = AutoId( istr );
        Bind<int32_t>( &AttractableComponent::SetAttractType, typ );
    }
    Bind( "deceleration", func_double( &AttractableComponent::SetDeceleration ) );
    scriptedcontroller::TurnToTargetAct act;
    auto const& json = (*mSetters)["turn_to_target"];
    if (json.isObject())
    {
        act.Load( json );
        Bind<scriptedcontroller::TurnToTargetAct>( &AttractableComponent::SetTurnToTargetAct, act );
    }
}

AttractableComponentLoader::AttractableComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( AttractableComponent, AttractableComponent );
