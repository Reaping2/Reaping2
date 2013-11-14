#include "i_core.h"

PlasmaShot::PlasmaShot()
	:Shot("plasma_shot")
{
	SetSpeed(2);
	AddAction(AutoId("move"));
}