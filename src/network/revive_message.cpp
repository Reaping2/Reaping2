#include "network/revive_message.h"
#include "core/i_health_component.h"
#include "core/i_position_component.h"
#include "core/actor_factory.h"
#include "core/i_inventory_component.h"
#include "lifecycle_message_handler_sub_system.h"
namespace network {

    ReviveMessageSenderSystem::ReviveMessageSenderSystem()
        : MessageSenderSystem()
        , mIsRivevePressed(false)
    {
        mKeyId = EventServer<KeyEvent>::Get().Subscribe( boost::bind( &ReviveMessageSenderSystem::OnKeyEvent, this, _1 ) );
    }

    void ReviveMessageSenderSystem::Init()
    {
        MessageSenderSystem::Init();
        mIsRivevePressed=false;
        SetFrequency(10);
    }

    void ReviveMessageSenderSystem::Update(double DeltaTime)
    {
        MessageSenderSystem::Update(DeltaTime);
        if (!IsTime())
        {
            return;
        }
        if(!mIsRivevePressed)
        {
            return;
        }
        L2("handle revive request!\n");
        //TODO: might need optimization
        Opt<Actor> actor=mScene.GetActor(mProgramState.mControlledActorGUID);
        if (actor.IsValid())
        {
            Opt<IHealthComponent> healthC = actor->Get<IHealthComponent>();
            if (healthC.IsValid()&&!healthC->IsAlive())
            {
                std::auto_ptr<ReviveMessage> reviveMsg(new ReviveMessage);
                mMessageHolder.AddOutgoingMessage(reviveMsg);
                L2("revive message sent clientId: %d!\n",mProgramState.mClientId);
            }
            else
            {
                L1("health is not available, or actor still alive:%d\n",actor->GetGUID());
            }
        }
        mIsRivevePressed=false;
    }

    void ReviveMessageSenderSystem::OnKeyEvent(const KeyEvent& Event)
    {
        //TODO: i don't think this is the right place for this, maybe a global keys or something like that would be a better idea
        if( Event.Key == GLFW_KEY_SPACE && Event.State == KeyState::Up )
        {
            mIsRivevePressed=true;
            L2("space pressed for revive!\n");
        }
    }

    ReviveMessageHandlerSubSystem::ReviveMessageHandlerSubSystem()
        : MessageHandlerSubSystem()
        
    {

    }

    void ReviveMessageHandlerSubSystem::Init()
    {

    }

    void ReviveMessageHandlerSubSystem::Execute(Message const& message)
    {
        ReviveMessage const& msg=static_cast<ReviveMessage const&>(message);
        L2("got revive message: senderId:%d\n",msg.mSenderId);
        for (core::ProgramState::ClientDatas_t::iterator i=mProgramState.mClientDatas.begin(), e=mProgramState.mClientDatas.end();i!=e;++i)
        {
            if (i->mClientId==msg.mSenderId)
            {
                L2("found client for revive: senderId:%d\n",msg.mSenderId);
                if(!i->mClientActor.IsValid())
                {
                    L1("revive called and clientactor for %d is not valid!(error)\n",msg.mSenderId);
                    return;
                }
                Opt<IHealthComponent> healthC = i->mClientActor->Get<IHealthComponent>();
                if (healthC.IsValid()&&!healthC->IsAlive())
                {
                    std::auto_ptr<Actor> player(ActorFactory::Get()(AutoId("player")));
                    Opt<IPositionComponent> positionC = player->Get<IPositionComponent>();
                    glm::vec4 const& dimensions=mScene.GetDimensions();
                    positionC->SetX((dimensions.x + ( rand() % ( int )( ( dimensions.z - dimensions.x ) ) )) );
                    positionC->SetY((dimensions.y + ( rand() % ( int )( ( dimensions.w - dimensions.y ) ) )) );

                    //TODO: temporary till normal inventory sync 
                    Opt<IInventoryComponent> inventoryC = player->Get<IInventoryComponent>();
                    if (inventoryC.IsValid())
                    {
                        inventoryC->SetSelectedWeapon(AutoId( "plasma_gun" ));
                    }
                    LifecycleMessageHandlerSubSystem::AddNewPlayer(*i,player);
                }
                else
                {
                    L1("health is not available, or actor still alive:%d\n",i->mClientActor->GetGUID());
                }
            }
        }
        L2("end revive message: senderId:%d\n",msg.mSenderId);
    }

} // namespace engine

