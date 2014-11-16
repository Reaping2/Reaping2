#include "i_core.h"

PlasmaShot::PlasmaShot()
    : Shot( "plasma_shot" )
{
    mFields[RADIUS].d = 0.02;
    mFields[DAMAGE].i = 30;
}
