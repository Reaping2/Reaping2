#include "platform/i_platform.h"
#include "core/item.h"

DefaultItem::DefaultItem( int32_t Id )
    : Item( Id )
{
    mType = ItemType::Normal;
}

void Item::SetActorGUID( int32_t actorGUID )
{
    mActorGUID = actorGUID;
}

int32_t Item::GetActorGUID() const
{
    return mActorGUID;
}

Item::Item( int32_t Id )
    : mId( Id )
    , mState( 0 )
    , mActorGUID( 0 )
    , mType( ItemType::Normal )
{
    LOG( "!Item: id: %d\n", mId );
}

Item::Item()
    : mId( -1 )
    , mState( 0 )
    , mActorGUID( 0 )
    , mType( ItemType::Normal )
{

}

void DefaultItemLoader::BindValues()
{

}
DefaultItemLoader::DefaultItemLoader()
{
}
