#include "image.h"
#include <json/json.h>

ImageWidget::ImageWidget( int32_t Id )
    : Widget( Id )
{
}

void ImageWidget::Init( Json::Value& Descriptor )
{
    BaseClass::Init( Descriptor );
    ParseIntProp( PT_ActorVisual, Descriptor["actor_visual"], 0 );
    ParseIntProp( PT_Animation, Descriptor["animation"], ( int32_t ) AutoId( "idle" ) );
    ParseIntProp( PT_State, Descriptor["state"], 0 );
}


