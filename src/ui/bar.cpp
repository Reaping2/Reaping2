#include "i_ui.h"

Bar::Bar( int32_t Id )
    : Widget( Id )
{

}

void Bar::Init( Json::Value& Descriptor )
{
    BaseClass::Init( Descriptor );
    ParseIntProp( PT_MaxProgress, Descriptor["max_progress"], 100 );
    ParseIntProp( PT_Progress, Descriptor["progress"], 0 );
}

