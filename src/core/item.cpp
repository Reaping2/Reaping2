#include "platform/i_platform.h"
#include "core/item.h"

DefaultItem::DefaultItem( int32_t Id )
    : Item( Id )
{
    mType = ItemType::Normal;
}

void Item::SetActor( Actor* Obj )
{
    mActor = Obj;
}

Actor* Item::GetActor() const
{
    return mActor;
}

Item::Item( int32_t Id )
    : mId( Id )
    , mState( 0 )
{
    LOG( "!Item: id: %d\n", mId );
}

