#include "platform/i_platform.h"
#include "engine/collision_system.h"
#include "core/i_collision_component.h"
#include "boost/assert.hpp"
#include "core/collision_model.h"

namespace engine {

CollisionSystem::CollisionSystem()
    : mCollisionStore( CollisionStore::Get() )
    , mScene( Scene::Get() )
{

}

void CollisionSystem::Init()
{
    mCollisionGrid.Build( mScene.GetDimensions(), 400.0f );
}

void CollisionSystem::Update(double DeltaTime)
{
    mCollisionGrid.Clear();
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
        Actor& actor = **it;
        Opt<ICollisionComponent> collisionC=actor.Get<ICollisionComponent>();
        if (collisionC.IsValid())
        {
            mCollisionGrid.AddActor( &actor, DeltaTime );
            Opt<CollisionSubSystem> collisionSS=GetCollisionSubSystem(collisionC->GetId());
            if (collisionSS.IsValid())
            {
                collisionSS->ClipScene(actor);
            }
        }
    }
    PossibleCollisions_t const& PossibleCollisions = mCollisionGrid.GetPossibleCollisions();
    for( PossibleCollisions_t::const_iterator i = PossibleCollisions.begin(), e = PossibleCollisions.end(); i != e; ++i )
    {
        Actor& A = *( i->A1 );
        Actor& B = *( i->A2 );
        Opt<ICollisionComponent> ACollisionC = A.Get<ICollisionComponent>();
        Opt<ICollisionComponent> BCollisionC = B.Get<ICollisionComponent>();
        BOOST_ASSERT(ACollisionC.IsValid() && BCollisionC.IsValid()); //TODO: here this one should be true

        CollisionModel const& CollModel = mCollisionStore.GetCollisionModel( ACollisionC->GetCollisionClass(), BCollisionC->GetCollisionClass() );
        if( !CollModel.AreActorsColliding( A, B, DeltaTime ) )
        {
            continue;
        }
        //TODO: needs optimization, maybe a template parameter for SubSystemHolder to subsystem would do
        Opt<CollisionSubSystem> ACollisionSS=GetCollisionSubSystem(ACollisionC->GetId());
        if (ACollisionSS.IsValid())
        {
            ACollisionSS->SetOther(&B);
            ACollisionSS->Update(A,DeltaTime);
            ACollisionSS->SetOther(NULL);
        }
        Opt<CollisionSubSystem> BCollisionSS=GetCollisionSubSystem(BCollisionC->GetId());
        if (BCollisionSS.IsValid())
        {
            BCollisionSS->SetOther(&A);
            BCollisionSS->Update(B,DeltaTime);
            BCollisionSS->SetOther(NULL);
        }
    }

}

Opt<CollisionSubSystem> CollisionSystem::GetCollisionSubSystem(int32_t id)
{
    Opt<CollisionSubSystem> r;
    BindIds_t& bindIds=mSubSystems.get<SubSystemHolder::AllByBindId>();
    BindIds_t::iterator subsysIt=bindIds.find(id);
    if (subsysIt!=bindIds.end())
    {
        r=Opt<CollisionSubSystem>(
            dynamic_cast<CollisionSubSystem*>(
            subsysIt->mSystem.Get()));
    }
    return r;
}

} // namespace engine

