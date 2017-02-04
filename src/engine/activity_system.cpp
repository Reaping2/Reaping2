#include "platform/i_platform.h"
#include "engine/activity_system.h"
#include "boost/assert.hpp"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/i_static_actor_component.h"
#include "platform/settings.h"
#include <utility>

namespace engine {

ActivitySystem::ActivitySystem()
    : mScene( Scene::Get() )
{

}

void ActivitySystem::Init()
{
    static const auto gridsize = Settings::Get().GetDouble( "activity.gridsize", 500.0 );
    mActivityGrid.Build( mScene.GetDimensions(), gridsize );
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IPositionComponent>().IsValid()
            && actor.Get<IMoveComponent>().IsValid()
            && !actor.Get<IStaticActorComponent>().IsValid(); } );
    mOnActorEvent = EventServer<ActorEvent>::Get().Subscribe( boost::bind( &ActivitySystem::OnActorEvent, this, _1 ) );
}

void ActivitySystem::OnActorEvent( ActorEvent const& Evt )
{
    if( Evt.mState == ActorEvent::Added )
    {
        Opt<IPositionComponent> posC = Evt.mActor->Get<IPositionComponent>();
        if( !posC.IsValid() )
        {
            return;
        }
        if ( !Evt.mActor->Get<IMoveComponent>().IsValid() ||
             Evt.mActor->Get<IStaticActorComponent>().IsValid() )
        {
            mActivityGrid.AddActor( Evt.mActor.Get(), 0 );
            mDirty = true;
        }
    }
    else
    {
        mActivityGrid.RemoveActor( Evt.mActor.Get() );
        mDirty = true;
    }
}

namespace {
void adjust( double& minx, double& miny, double& maxx, double& maxy, Actor& a )
{
    auto posC = a.Get<IPositionComponent>();
    if( !posC.IsValid() )
    {
        return;
    }
    double r = core::ActivityTraits::GetRadius( a );
    double ix = posC->GetX() - r;
    double ax = ix + 2. * r;
    double iy = posC->GetY() - r;
    double ay = iy + 2. * r;
    if( minx > ix )
    {
        minx = ix;
    }
    if( maxx < ax )
    {
        maxx = ax;
    }
    if( miny > iy )
    {
        miny = iy;
    }
    if( maxy < ay )
    {
        maxy = ay;
    }
}
}

bool ActivitySystem::ActorOrderer::operator()( Actor const* a, Actor const* b ) const
{
    if( a == b )
    {
        return false;
    }
    auto arC = a->Get<IRenderableComponent>();
    auto brC = b->Get<IRenderableComponent>();
    if( !arC.IsValid() && !brC.IsValid() )
    {
        return a->GetGUID() < b->GetGUID();
    }
    if( !arC.IsValid() )
    {
        return true;
    }
    return arC->GetLayerPriority() < brC->GetLayerPriority() ||
        ( arC->GetLayerPriority() == brC->GetLayerPriority() &&
          ( arC->GetZOrder() < brC->GetZOrder() ||
            ( arC->GetZOrder() == brC->GetZOrder() &&
              ( arC->GetReceiveShadow() > brC->GetReceiveShadow() ||
                ( arC->GetReceiveShadow() == brC->GetReceiveShadow() &&
                  ( a->GetId() < b->GetId() ||
                    ( a->GetId() == b->GetId() &&
                      a->GetGUID() < b->GetGUID() ) ) ) ) ) ) );
}

ActivitySystem::OrderedActors_t const& ActivitySystem::GetActiveActors()
{
    if( mDirty )
    {
        mDirty = false;
        double minx = std::numeric_limits<double>::max();
        double maxx = std::numeric_limits<double>::min();
        double miny = std::numeric_limits<double>::max();
        double maxy = std::numeric_limits<double>::min();
        mActiveActors.clear();
        auto funptr = static_cast<void(OrderedActors_t::*)(ActivityGrid::Actors_t::const_iterator,ActivityGrid::Actors_t::const_iterator)>( &OrderedActors_t::insert );
        auto insertfun = std::bind( funptr, &mActiveActors, std::placeholders::_1, std::placeholders::_2 );
        mActivityGrid.CollectActorsWithMaskAndAround( 1 << core::ActivityTraits::Active, insertfun );
        for( auto a : mActiveActors )
        {
            adjust( minx, miny, maxx, maxy, *a );
        }
        mActiveRegion = glm::vec4( minx, miny, maxx, maxy );
    }
    return mActiveActors;
}

glm::vec4 const& ActivitySystem::GetActiveRegion()
{
    if( mDirty )
    {
        GetActiveActors();
    }
    return mActiveRegion;
}

void ActivitySystem::Update( double DeltaTime )
{
    mUpdateTimer.Log( "start activity" );
    mPerfTimer.Log( "pre build grid" );
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
        mActivityGrid.AddActor( actor, DeltaTime );
    }
    mDirty = true;
    mUpdateTimer.Log( "end activity" );
}


} // namespace engine

