#include "image.h"
#include <json/json.h>

ImageWidget::ImageWidget( int32_t Id )
    : Widget( Id )
{
}

void ImageWidget::Init( Json::Value& Descriptor )
{
    BaseClass::Init( Descriptor );
    ParseStrProp( PT_ActorVisual, Descriptor["actor_visual"], "" );
    ParseStrProp( PT_Animation, Descriptor["animation"], "body_idle" );
    ParseIntProp( PT_State, Descriptor["state"], 0 );
}


