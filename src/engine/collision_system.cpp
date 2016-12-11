#include "platform/i_platform.h"
#include "engine/collision_system.h"
#include "core/i_collision_component.h"
#include "boost/assert.hpp"
#include "core/collision_model.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "platform/settings.h"
#include "core/box_collision_model.h"
#include "core/program_state.h"
#include "core/i_renderable_component.h"
#include <utility>

namespace engine {

CollisionSystem::CollisionSystem()
    : mCollisionStore( CollisionStore::Get() )
    , mScene( Scene::Get() )
{

}

void CollisionSystem::Init()
{
    SubSystemHolder::Init();
    mCollisionGrid.Build( mScene.GetDimensions(), 400.0f );
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<ICollisionComponent>().IsValid()
            && actor.Get<IPositionComponent>().IsValid()
            && actor.Get<IMoveComponent>().IsValid(); } );
    mOnActorEvent = EventServer<ActorEvent>::Get().Subscribe( boost::bind( &CollisionSystem::OnActorEvent, this, _1 ) );
    mOnMouseMoveEvent = EventServer<WorldMouseMoveEvent>::Get().Subscribe( boost::bind( &CollisionSystem::OnMouseMoveEvent, this, _1 ) );
}

void CollisionSystem::OnMouseMoveEvent( WorldMouseMoveEvent const& Evt )
{
    mMousePos = Evt.Pos;
}

void CollisionSystem::OnActorEvent( ActorEvent const& Evt )
{
    if( Evt.mState == ActorEvent::Added )
    {
        Opt<ICollisionComponent> collisionC = Evt.mActor->Get<ICollisionComponent>();
        if( !collisionC.IsValid() )
        {
            return;
        }
        if ( !Evt.mActor->Get<IMoveComponent>().IsValid() )
        {
            mCollisionGrid.AddActor( Evt.mActor.Get(), 0, collisionC );
        }
    }
    else
    {
        mCollisionGrid.RemoveActor( Evt.mActor.Get() );
    }
}

namespace {
void paint( Actor* a, glm::vec4 const& col )
{
    if( nullptr == a )
    {
        return;
    }
    Opt<IRenderableComponent> rc = a->Get<IRenderableComponent>();
    if( !rc.IsValid() )
    {
        return;
    }
    rc->SetColor( col );
}
glm::vec2 dir( Actor& a, glm::vec2 const& pt )
{
    Opt<IPositionComponent> pc = a.Get<IPositionComponent>();
    if( !pc.IsValid() )
    {
        return glm::vec2();
    }
    return pt - glm::vec2( pc->GetX(), pc->GetY() );
}
}

void CollisionSystem::Update( double DeltaTime )
{
    static Actor* coll = nullptr;
    paint( coll, glm::vec4( 1,1,1,1 ) );
    static core::ProgramState& ps( core::ProgramState::Get() );
    Opt<Actor> player = mScene.GetActor( ps.mControlledActorGUID );
    if( player.IsValid() )
    {
        coll = GetFirstCollidingActor( *player, dir( *player, mMousePos ) ).Get();
        paint( coll, glm::vec4( 0,1,0,0.2 ) );
    }
    static const auto enableCollision = Settings::Get().GetInt( "collisions.enable", 1 ) != 0;
    if( !enableCollision )
    {
        return;
    }
    mUpdateTimer.Log( "start collision" );
    mPerfTimer.Log( "pre build grid" );
    std::vector<std::pair<Opt<CollisionSubSystem>, Actor*>> collisionAndActors;
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
        Opt<ICollisionComponent> collisionC = actor->Get<ICollisionComponent>();
        if ( collisionC.IsValid() )
        {
            Opt<CollisionSubSystem> collisionSS = GetCollisionSubSystem( collisionC->GetId() );
            if ( collisionSS.IsValid() )
            {
                collisionAndActors.push_back( std::make_pair( collisionSS, actor ) );
                collisionSS->ClipScene( *actor );
            }
            mCollisionGrid.AddActor( actor, DeltaTime, collisionC );
        }
    }
    mPerfTimer.Log( "post build grid" );
    PossibleCollisions_t const& PossibleCollisions = mCollisionGrid.GetPossibleCollisions();
    for( PossibleCollisions_t::const_iterator i = PossibleCollisions.begin(), e = PossibleCollisions.end(); i != e; ++i )
    {
        Actor& A = *( i->A1 );
        Actor& B = *( i->A2 );
        Opt<ICollisionComponent> ACollisionC = A.Get<ICollisionComponent>();
        Opt<ICollisionComponent> BCollisionC = B.Get<ICollisionComponent>();
        BOOST_ASSERT( ACollisionC.IsValid() && BCollisionC.IsValid() ); //TODO: here this one should be true

        CollisionModel const& CollModel = mCollisionStore.GetCollisionModel( ACollisionC->GetCollisionClass(), BCollisionC->GetCollisionClass() );
        if( !CollModel.AreActorsColliding( A, B, DeltaTime ) )
        {
            continue;
        }
        //TODO: needs optimization, maybe a template parameter for SubSystemHolder to subsystem would do
        Opt<CollisionSubSystem> ACollisionSS = GetCollisionSubSystem( ACollisionC->GetId() );
        if ( ACollisionSS.IsValid() )
        {
            ACollisionSS->Collide( A, B );
        }
        Opt<CollisionSubSystem> BCollisionSS = GetCollisionSubSystem( BCollisionC->GetId() );
        if ( BCollisionSS.IsValid() )
        {
            BCollisionSS->Collide( B, A );
        }
    }
    mPerfTimer.Log( "post collide" );
    for (auto& collAndActor : collisionAndActors)
    {
        collAndActor.first->Update( *collAndActor.second, DeltaTime );
    }
    mPerfTimer.Log( "post collSS" );
    mUpdateTimer.Log( "end collision" );
}

Opt<CollisionSubSystem> CollisionSystem::GetCollisionSubSystem( int32_t id )
{
    BindIds_t& bindIds = mSubSystems.get<SubSystemHolder::AllByBindId>();
    BindIds_t::iterator subsysIt = bindIds.find( id );
    if ( subsysIt != bindIds.end() )
    {
        return Opt<CollisionSubSystem>(
                static_cast<CollisionSubSystem*>(
                    subsysIt->mSystem.Get() ) );
    }
    return nullptr;
}

std::set<Actor*> CollisionSystem::GetAllCollidingActors( glm::vec2 const& position, double radius, int32_t collMask ) const
{
    CollisionModel::Object ObjA{ position, glm::vec2(), radius };
    std::set<Actor*> &&all( mCollisionGrid.GetAllNearbyActors( position, radius, collMask ) ), rv;
    for( auto actor : all )
    {
        static BoxCollisionModel collModel;
        CollisionModel::Object ObjB( CollisionModel::ObjectFromActor( *actor ) );
        if( collModel.AreActorsColliding( ObjA, ObjB, 0.0 ) )
        {
            rv.insert( actor );
        }
    }
    return std::move( rv );
}

namespace {
double distance( CollisionModel::Object const& a, CollisionModel::Object const& b )
{
    return std::max( 0.0, glm::distance( a.position, b.position ) - a.radius - b.radius );
}
}

Opt<Actor> CollisionSystem::GetFirstCollidingActor( Actor const& actor, glm::vec2 const& direction, double radius, int32_t collMask ) const
{
    CollisionModel::Object ObjA( CollisionModel::ObjectFromActor( actor ) );
    if( radius > -0.1 )
    {
        ObjA.radius = std::abs( radius );
    }
    ObjA.speed = glm::normalize( direction );
    std::set<Actor*> &&all( mCollisionGrid.GetAllNearbyActors( ObjA.position, ObjA.radius, collMask, &direction ) ), rv;
    all.erase( const_cast<Actor*>(&actor) );
    Actor* closest = nullptr;
    double dist = std::numeric_limits<double>::max();
    for( auto act : all )
    {
        static BoxCollisionModel collModel;
        CollisionModel::Object ObjB( CollisionModel::ObjectFromActor( *act ) );
        ObjB.speed = glm::vec2();
        double d = distance( ObjA, ObjB );
        if( ( d < dist || nullptr == closest ) &&
            collModel.AreActorsColliding( ObjA, ObjB, 10e8 ) )
        {
            dist = d;
            closest = act;
        }
    }
    return Opt<Actor>( closest );
}

bool CollisionSystem::IsColliding( Actor const& actor ) const
{
    Opt<ICollisionComponent> collisionC = actor.Get<ICollisionComponent>();
    if ( !collisionC.IsValid() )
    {
        return false;
    }
    std::set<Actor*>&& all( mCollisionGrid.GetAllNearbyActors( &actor ) );
    for( auto act : all )
    {
        if( act == &actor )
        {
            continue;
        }
        Opt<ICollisionComponent> ACollisionC = act->Get<ICollisionComponent>();
        CollisionModel const& CollModel = mCollisionStore.GetCollisionModel( ACollisionC->GetCollisionClass(), collisionC->GetCollisionClass() );
        if( CollModel.AreActorsColliding( actor, *act, 0.0 ) )
        {
            return true;
        }
    }
    return false;
}


} // namespace engine

