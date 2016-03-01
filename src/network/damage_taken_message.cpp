#include "network/damage_taken_message.h"
#include "core/player_controller_component.h"
#include "core/i_health_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
namespace network {

    DamageTakenMessageSenderSystem::DamageTakenMessageSenderSystem()
        : MessageSenderSystem()
    {

    }

    void DamageTakenMessageSenderSystem::Init()
    {
        MessageSenderSystem::Init();
        mOnDamageTaken= EventServer<core::DamageTakenEvent>::Get().Subscribe( boost::bind( &DamageTakenMessageSenderSystem::OnDamageTaken, this, _1 ) );

    }

    void DamageTakenMessageSenderSystem::Update(double DeltaTime)
    {
        MessageSenderSystem::Update(DeltaTime);
    }

    void DamageTakenMessageSenderSystem::OnDamageTaken(core::DamageTakenEvent const& Evt)
    {
        std::auto_ptr<DamageTakenMessage> damageTakenMsg(new DamageTakenMessage);
        damageTakenMsg->mActorGUID=Evt.ActorGUID;
        damageTakenMsg->mX=Evt.Pos.x*PRECISION;
        damageTakenMsg->mY=Evt.Pos.y*PRECISION;
        damageTakenMsg->mDamage=Evt.Damage;
        damageTakenMsg->mType=Evt.type;
        mMessageHolder.AddOutgoingMessage(damageTakenMsg);
    }

    DamageTakenMessageHandlerSubSystem::DamageTakenMessageHandlerSubSystem()
        : MessageHandlerSubSystem()
    {

    }

    void DamageTakenMessageHandlerSubSystem::Init()
    {

    }

    void DamageTakenMessageHandlerSubSystem::Execute(Message const& message)
    {
        DamageTakenMessage const& msg=static_cast<DamageTakenMessage const&>(message);
        Opt<Actor> actor=mScene.GetActor(msg.mActorGUID);
        if (!actor.IsValid())
        {
            L1("cannot find actor with GUID: (damge taken) %d \n",msg.mActorGUID );
            return;
        }

        Opt<IHealthComponent> healthC = actor->Get<IHealthComponent>();
        if (!healthC.IsValid())
        {
            L1("damage taken on an actor withot heal component \n" );
            return;
        }
        healthC->TakeDamage(msg.mDamage);
    }

} // namespace engine


REAPING2_CLASS_EXPORT_IMPLEMENT(network__DamageTakenMessage, network::DamageTakenMessage);
