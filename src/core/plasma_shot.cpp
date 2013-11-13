#include "i_core.h"

PlasmaShot::PlasmaShot()
	:Shot("plasma_shot")
{
	SetSpeed(2);
	ActionRepo::Get()(AutoId("move"))->Activate(*this);
}