#include "platform/i_platform.h"
#include "network/collision_message.h"
#include "core/i_collision_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {

CollisionMessageSenderSystem::CollisionMessageSenderSystem()
    : MessageSenderSystem()
{
}


void CollisionMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
}


void CollisionMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

CollisionMessageHandlerSubSystem::CollisionMessageHandlerSubSystem()
    : PendingMessageHandlerSubSystem()
{
}


void CollisionMessageHandlerSubSystem::Init()
{
}

void CollisionMessageHandlerSubSystem::Update(double DeltaTime)
{
    PendingMessageHandlerSubSystem::Update(DeltaTime);
}

bool CollisionMessageHandlerSubSystem::ProcessPending(Message const& message)
{
    CollisionMessage const& msg=static_cast<CollisionMessage const&>(message);
    Opt<Actor> actor=mScene.GetActor(msg.mActorGUID); //guaranteed
    //L1("executing %s: actorGUID %d \n",__FUNCTION__,msg.mActorGUID );
    Opt<ICollisionComponent> collisionC = actor->Get<ICollisionComponent>();
    if (!collisionC.IsValid())
    {
        L1("collisionMessage arrived for actor without i_collision_component actorGUID %d \n",msg.mActorGUID );
        return true;
    }
    collisionC->SetCollisionClass(msg.mCollisionClass);
    collisionC->SetRadius(msg.mRadius);
    return true;
}
 
std::auto_ptr<CollisionMessage> CollisionMessageSenderSystem::GenerateCollisionMessage(Actor &actor)
{
    Opt<ICollisionComponent> collisionC = actor.Get<ICollisionComponent>();
    if (!collisionC.IsValid())
    {
        return std::auto_ptr<CollisionMessage>();
    }
    std::auto_ptr<CollisionMessage> collisionMessage(new CollisionMessage);
    collisionMessage->mActorGUID=actor.GetGUID();
    collisionMessage->mCollisionClass=collisionC->GetCollisionClass();
    collisionMessage->mRadius=collisionC->GetRadius();
    return collisionMessage;

}

} // namespace network


BOOST_CLASS_EXPORT_IMPLEMENT(network::CollisionMessage);
