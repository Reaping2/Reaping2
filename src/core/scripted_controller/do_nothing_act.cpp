#include "do_nothing_act.h"
#include "core/i_move_component.h"
#include "core/i_position_component.h"

namespace scriptedcontroller
{

void DoNothingAct::Update( Actor& actor, double Seconds )
{
    IAct::Update( actor, Seconds );
}

void DoNothingAct::Load( Json::Value const& setters )
{
    IAct::Load( setters );
}


void DoNothingAct::Start( Actor& actor )
{
    IAct::Start( actor );
}

void DoNothingAct::Stop( Actor& actor )
{
    IAct::Stop( actor );
}


} // namespace scriptedcontroller

