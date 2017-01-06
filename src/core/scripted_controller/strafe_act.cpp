#include "strafe_act.h"
#include "core/i_move_component.h"
#include "core/i_position_component.h"

namespace scriptedcontroller
{

void StrafeAct::Update( Actor& actor, double Seconds )
{
    IAct::Update( actor, Seconds );
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    if (!moveC.IsValid())
    {
        return;
    }
    auto positionC( actor.Get<IPositionComponent>() );
    if (!positionC.IsValid())
    {
        return;
    }

    moveC->SetHeading( positionC->GetOrientation()
        + mHeadingRotation );
}

void StrafeAct::Load( Json::Value const& setters )
{
    IAct::Load( setters );
    Json::GetDouble( setters["speed"], mSpeed );
}


void StrafeAct::Start( Actor& actor )
{
    IAct::Start( actor );
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    if (!moveC.IsValid())
    {
        return;
    }
    moveC->GetSpeed().mBase.Set( mSpeed );
    moveC->SetMoving( true );
    mHeadingRotation = boost::math::constants::pi<double>()/4*(RandomGenerator::global()() % 2 == 1 ? 1:-1);
}

void StrafeAct::Stop( Actor& actor )
{
    IAct::Stop( actor );
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    if (!moveC.IsValid())
    {
        return;
    }
    moveC->GetSpeed().mBase.Set( 0.0 );
    moveC->SetMoving( false );
}


} // namespace scriptedcontroller

