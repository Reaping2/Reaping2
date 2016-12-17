#include "freelance_act.h"
#include "core/i_move_component.h"
#include "core/i_position_component.h"

namespace scriptedcontroller
{

void FreelanceAct::Update( Actor& actor, double Seconds )
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
    actor.Get<IPositionComponent>()->SetOrientation( moveC->GetHeading() );
}

void FreelanceAct::Load( Json::Value const& setters )
{
    IAct::Load( setters );
}


void FreelanceAct::Start( Actor& actor )
{
    IAct::Start( actor );
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    if (!moveC.IsValid())
    {
        return;
    }
    moveC->GetSpeed().mBase.Set( (RandomGenerator::global()() % 10) * 30 );
    moveC->SetMoving( moveC->GetSpeed().Get() != 0 );

    moveC->SetHeadingModifier( 1 );
}

void FreelanceAct::Stop( Actor& actor )
{
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    if (!moveC.IsValid())
    {
        return;
    }
    moveC->GetSpeed().mBase.Set( 0.0 );
    moveC->SetMoving( false );
    moveC->SetHeadingModifier( 0.0 );
    IAct::Stop( actor );
}


} // namespace scriptedcontroller

