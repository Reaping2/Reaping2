#include "stay_act.h"
#include "core/i_move_component.h"
#include "core/i_position_component.h"

namespace scriptedcontroller
{

void StayAct::Update( Actor& actor, double Seconds )
{
    IAct::Update( actor, Seconds );
}

void StayAct::Load( Json::Value const& setters )
{
    IAct::Load( setters );
}


void StayAct::Start( Actor& actor )
{
    IAct::Start( actor );
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    if (!moveC.IsValid())
    {
        return;
    }
    moveC->GetSpeed().mBase.Set( 0.0 );
    moveC->SetMoving( false );
    moveC->SetHeadingModifier( 0.0 );
}

void StayAct::Stop( Actor& actor )
{
    IAct::Stop( actor );
}


} // namespace scriptedcontroller

