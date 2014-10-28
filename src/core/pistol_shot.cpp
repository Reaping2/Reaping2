#include "i_core.h"

PistolShot::PistolShot()
    : Shot( "pistol_shot" )
{
    mFields[RADIUS].d = 0.01;
    mFields[DAMAGE].i = 1;
}

