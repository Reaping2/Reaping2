#include "platform/i_platform.h"
#include "network/sync_item_message.h"
#include "core/i_inventory_component.h"

namespace network {


SyncItemMessage::SyncItemMessage( Item const& item )
    : mActorGUID( 0 )
    , mItemID( item.GetId() )
    , mData()
{
    if( NULL != item.GetActor() )
    {
        mActorGUID = item.GetActor()->GetGUID();
    }
    std::ostringstream oss;
    eos::portable_oarchive oa(oss);
    item.serialize( oa );
    mData = oss.str();
}

SyncItemMessageSenderSystem::SyncItemMessageSenderSystem()
    : MessageSenderSystem()
{
}


void SyncItemMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnItemPropertiesChanged=EventServer<engine::ItemPropertiesChangedEvent>::Get().Subscribe( boost::bind( &SyncItemMessageSenderSystem::OnItemPropertiesChanged, this, _1 ) );
}


void SyncItemMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void SyncItemMessageSenderSystem::OnItemPropertiesChanged(engine::ItemPropertiesChangedEvent const& Evt)
{
    std::auto_ptr<SyncItemMessage> syncItemMsg(new SyncItemMessage( Evt.mItem ) );
    mMessageHolder.AddOutgoingMessage(syncItemMsg);
}

SyncItemMessageHandlerSubSystem::SyncItemMessageHandlerSubSystem()
    : PendingMessageHandlerSubSystem()
{
}


void SyncItemMessageHandlerSubSystem::Init()
{
}

void SyncItemMessageHandlerSubSystem::Update(double DeltaTime)
{
    PendingMessageHandlerSubSystem::Update(DeltaTime);
}

bool SyncItemMessageHandlerSubSystem::ProcessPending(Message const& message)
{
    SyncItemMessage const& msg=static_cast<SyncItemMessage const&>(message);
    Opt<Actor> actor=mScene.GetActor(msg.mActorGUID); //guaranteed
    L1("executing %s: actorGUID %d \n",__FUNCTION__,msg.mActorGUID );
    Opt<IInventoryComponent> inv = actor->Get<IInventoryComponent>();
    if( !inv.IsValid() )
    {
        return false;
    }
    Opt<Item> item = inv->GetItem( msg.mItemID );
    if( !item.IsValid() )
    {
        return false;
    }
    std::istringstream iss( msg.mData );
    eos::portable_iarchive ia(iss);
    item->serialize( ia );
    return true;
}

} // namespace network

