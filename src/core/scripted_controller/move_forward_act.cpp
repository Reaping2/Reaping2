#include "move_forward_act.h"
#include "core/i_move_component.h"
#include "core/i_position_component.h"

namespace scriptedcontroller
{

void MoveForwardAct::Update( Actor& actor, double Seconds )
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
    moveC->SetHeading( positionC->GetOrientation() );
}

void MoveForwardAct::Load( Json::Value const& setters )
{
    IAct::Load( setters );
    Json::GetDouble( setters["speed"], mSpeed );
}


void MoveForwardAct::Start( Actor& actor )
{
    IAct::Start( actor );
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    if (!moveC.IsValid())
    {
        return;
    }
    if (mSpeed >= 0.0)
    {
        moveC->GetSpeed().mBase.Set( mSpeed );
    }
    moveC->SetMoving( true );

}

void MoveForwardAct::Stop( Actor& actor )
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

