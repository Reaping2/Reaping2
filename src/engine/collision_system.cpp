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
    mCollisionGrid.Build( mScene.GetDimensions(), 0.4f );
}

void CollisionSystem::Update(double DeltaTime)
{
    mCollisionGrid.Clear();
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
        Actor& Obj = **it;
        if (Obj.Get<ICollisionComponent>().IsValid())
        {
            mCollisionGrid.AddActor( &Obj, DeltaTime );
        }
    }
    PossibleCollisions_t const& PossibleCollisions = mCollisionGrid.GetPossibleCollisions();
    for( PossibleCollisions_t::const_iterator i = PossibleCollisions.begin(), e = PossibleCollisions.end(); i != e; ++i )
    {
        Actor& A = *( i->A1 );
        Actor& B = *( i->A2 );
        Opt<ICollisionComponent> ACollisionC = A.Get<ICollisionComponent>();
        Opt<ICollisionComponent> BcollisionC = B.Get<ICollisionComponent>();
        BOOST_ASSERT(ACollisionC.IsValid() && BcollisionC.IsValid()); //TODO: here this one should be true

        CollisionModel const& CollModel = mCollisionStore.GetCollisionModel( ACollisionC->GetCollisionClass(), BcollisionC->GetCollisionClass() );
        if( !CollModel.AreActorsColliding( A, B, DeltaTime ) )
        {
            continue;
        }
        ACollisionC->Collide( B );
        BcollisionC->Collide( A );
    }

}

} // namespace engine

