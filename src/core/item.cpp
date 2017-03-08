#include "platform/i_platform.h"
#include "core/item.h"

DefaultItem::DefaultItem( int32_t Id )
    : Item( Id )
{
    mType = ItemType::Normal;
}


Item::~Item()
{
    LOG( "Item dead:%i", mId );
}

void Item::SetActorGUID( int32_t actorGUID )
{
    mActorGUID = actorGUID;
}


bool Item::CanSwitch() const
{
    return true;
}


void Item::Deselected()
{
}


void Item::Selected()
{
}

ItemType::Type Item::GetType() const
{
    return mType;
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


double Item::GetState() const
{
    return mState;
}


void Item::SetState( double S )
{
    mState = S;
}


int32_t Item::GetId() const
{
    return mId;
}

void DefaultItemLoader::BindValues()
{

}
DefaultItemLoader::DefaultItemLoader()
{
}
