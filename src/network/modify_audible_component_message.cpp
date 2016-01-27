#include "platform/i_platform.h"
#include "network/modify_audible_component_message.h"
#include "core/i_audible_component.h"

namespace network {

ModifyAudibleComponentMessageSenderSystem::ModifyAudibleComponentMessageSenderSystem()
    : MessageSenderSystem()
{
}


void ModifyAudibleComponentMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnAudible=EventServer<core::AudibleEvent>::Get().Subscribe( boost::bind( &ModifyAudibleComponentMessageSenderSystem::OnAudible, this, _1 ) );
}


void ModifyAudibleComponentMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void ModifyAudibleComponentMessageSenderSystem::OnAudible(core::AudibleEvent const& Evt)
{
    std::auto_ptr<ModifyAudibleComponentMessage> modifyAudibleComponentMsg(new ModifyAudibleComponentMessage);
    modifyAudibleComponentMsg->mActorGUID=Evt.mActorGUID;
    modifyAudibleComponentMsg->mId=Evt.mId;
    modifyAudibleComponentMsg->mIsOneShot=Evt.mIsOneShot;
    modifyAudibleComponentMsg->mIsAddition=Evt.mIsAddition;
    mMessageHolder.AddOutgoingMessage(modifyAudibleComponentMsg);
    L1("added %s %d %d ", __FUNCTION__,Evt.mActorGUID,Evt.mId);
}

ModifyAudibleComponentMessageHandlerSubSystem::ModifyAudibleComponentMessageHandlerSubSystem()
    : PendingMessageHandlerSubSystem()
{
}


void ModifyAudibleComponentMessageHandlerSubSystem::Init()
{
}

void ModifyAudibleComponentMessageHandlerSubSystem::Update(double DeltaTime)
{
    PendingMessageHandlerSubSystem::Update(DeltaTime);
}

bool ModifyAudibleComponentMessageHandlerSubSystem::ProcessPending(Message const& message)
{
    ModifyAudibleComponentMessage const& msg=static_cast<ModifyAudibleComponentMessage const&>(message);
    Opt<Actor> actor=mScene.GetActor(msg.mActorGUID); //guaranteed
    L1("executing %s: actorGUID %d \n",__FUNCTION__,msg.mActorGUID );
    Opt<IAudibleComponent> ac = actor->Get<IAudibleComponent>();
    if( !ac.IsValid() )
    {
        BOOST_ASSERT( false );
        return true;
    }
    if( msg.mIsAddition )
    {
        if( msg.mIsOneShot )
        {
            ac->AddOneShotEffect( msg.mId );
        }
        else
        {
            ac->AddLoopingEffect( msg.mId );
        }
    }
    else
    {
        auto& evts = ac->GetEffects();
        std::for_each( std::begin( evts ), std::end( evts ),
                [&]( AudibleEffectDesc& desc ) { if( desc.Id == msg.mId && desc.TTL != AudibleEffectDesc::TTL_Infinity ) desc.TTL = 0; } );
    }
    return true;
}

} // namespace network

