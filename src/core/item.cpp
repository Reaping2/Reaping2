#include "platform/i_platform.h"
#include "core/item.h"

DefaultItem::DefaultItem( int32_t Id )
    : Item( Id )
{
    mType = Normal;
}

void Item::SetActor( Actor* Obj )
{
    mActor = Obj;
}
void DefaultItem::Update( double Seconds )
{
}


Item::Item( int32_t Id )
    : mId( Id )
    , mState( 0 )
{
    LOG( "!Item: id: %d\n", mId );
}


void Item::Update( double Seconds )
{
    if( !mActor )
    {
        return;
    }
}
