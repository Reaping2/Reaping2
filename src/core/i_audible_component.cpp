#include "i_audible_component.h"

int32_t const AudibleEffectDesc::TTL_Infinity = -100;

AudibleEffectDesc::AudibleEffectDesc( int32_t id )
        : UID( 0 ), Id ( id ), TTL( TTL_Infinity )
{
    static int32_t uid = 0;
    UID = ++uid;
}

