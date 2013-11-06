#include "i_core.h"

ShootAction::ShootAction()
	:Action("shoot")
{
}

void ShootAction::Update( Actor& Actor,double Seconds ) const
{
	Action::Update(Actor,Seconds);
}
