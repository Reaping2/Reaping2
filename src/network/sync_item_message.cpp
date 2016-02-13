#include "platform/i_platform.h"
#include "network/sync_item_message.h"
#include "core/i_inventory_component.h"

namespace network {


SyncItemMessage::SyncItemMessage( Item const& item )
    : mActorGUID( 0 )
    , mItemID( item.GetId() )
    , mData()
{
    mActorGUID = item.GetActorGUID();
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

void SyncItemMessageHandlerSubSystem::Init()
{
}

void SyncItemMessageHandlerSubSystem::Execute(Message const& message)
{
    SyncItemMessage const& msg=static_cast<SyncItemMessage const&>(message);
    Opt<Actor> actor=mScene.GetActor(msg.mActorGUID);
    if( !actor.IsValid() )
    {
        BOOST_ASSERT( false );
        return;
    }
    L1("executing %s: actorGUID %d \n",__FUNCTION__,msg.mActorGUID );
    Opt<IInventoryComponent> inv = actor->Get<IInventoryComponent>();
    if( !inv.IsValid() )
    {
        return;
    }
    Opt<Item> item = inv->GetItem( msg.mItemID );
    if( !item.IsValid() )
    {
        return;
    }
    std::istringstream iss( msg.mData );
    eos::portable_iarchive ia(iss);
    item->serialize( ia );
}

} // namespace network

