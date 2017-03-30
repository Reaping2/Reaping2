#include "platform/i_platform.h"
#include "network/dark_matter_message.h"
#include "core/i_inventory_component.h"
#include "platform/settings.h"

namespace network {

bool DarkMatterMessage::operator==( DarkMatterMessage const& other )
{
    return mActorGUID == other.mActorGUID
        && mDarkMatters == other.mDarkMatters;
}

void DarkMatterMessageSenderSystem::AddUniqueMessage( Actor& actor )
{
    mUniqueMessageSender.Add( actor.GetGUID(), GenerateDarkMatterMessage( actor ) );
}

void DarkMatterMessageSenderSystem::AddMandatoryMessage( Actor& actor )
{
    mMessageHolder.AddOutgoingMessage( GenerateDarkMatterMessage( actor ) );
}

DarkMatterMessageSenderSystem::DarkMatterMessageSenderSystem()
    : ActorTimerMessageSenderSystem( AutoId( "dark_matter" ) )
{
}


void DarkMatterMessageSenderSystem::Init()
{
    ActorTimerMessageSenderSystem::Init();
    SetFrequency( Settings::Get().GetDouble( "network.frequency.dark_matter", 0.01 ) );
}


void DarkMatterMessageSenderSystem::Update(double DeltaTime)
{
    ActorTimerMessageSenderSystem::Update( DeltaTime );
}

DarkMatterMessageHandlerSubSystem::DarkMatterMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void DarkMatterMessageHandlerSubSystem::Init()
{
}

void DarkMatterMessageHandlerSubSystem::Update(double DeltaTime)
{
    MessageHandlerSubSystem::Update(DeltaTime);
}

void DarkMatterMessageHandlerSubSystem::Execute(Message const& message)
{
    DarkMatterMessage const& msg = static_cast<DarkMatterMessage const&>(message);
    auto actor = mScene.GetActor( msg.mActorGUID );
    if (!actor.IsValid())
    {
        L1("cannot find actor with GUID: (%s) %d \n",__FUNCTION__,msg.mActorGUID );
        return;
    }
    auto inventoryC = actor->Get<IInventoryComponent>();
    if (!inventoryC.IsValid())
    {
        return;
    }
    L2( "Set DarkMatters!:%d\n", msg.mDarkMatters );
    inventoryC->SetDarkMatters( msg.mDarkMatters );
}

std::auto_ptr<DarkMatterMessage> DarkMatterMessageSenderSystem::GenerateDarkMatterMessage(Actor &actor)
{
    auto inventoryC = actor.Get<IInventoryComponent>();
    if (!inventoryC.IsValid())
    {
        return std::auto_ptr<DarkMatterMessage>();
    }
    std::auto_ptr<DarkMatterMessage> darkMatterMsg(new DarkMatterMessage);
    darkMatterMsg->mActorGUID=actor.GetGUID();
    darkMatterMsg->mDarkMatters=inventoryC->GetDarkMatters();
    return darkMatterMsg;
}

} // namespace network

REAPING2_CLASS_EXPORT_IMPLEMENT( network__DarkMatterMessage, network::DarkMatterMessage );

