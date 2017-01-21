#include "platform/i_platform.h"
#include "engine/activity_system.h"
#include "boost/assert.hpp"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
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
            && actor.Get<IMoveComponent>().IsValid(); } );
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
        if ( !Evt.mActor->Get<IMoveComponent>().IsValid() )
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
    return arC->GetLayer() < brC->GetLayer() ||
        ( arC->GetLayer() == brC->GetLayer() &&
          ( arC->GetZOrder() < brC->GetZOrder() ||
            ( arC->GetZOrder() == brC->GetZOrder() &&
              a->GetGUID() < b->GetGUID() ) ) );
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
        auto const& PossibleActivitys = mActivityGrid.GetPossibleCollisions();
        mActiveActors.clear();
        for( auto i = PossibleActivitys.begin(), e = PossibleActivitys.end(); i != e; ++i )
        {
            mActiveActors.insert( i->A1 );
            mActiveActors.insert( i->A2 );
            adjust( minx, miny, maxx, maxy, *(i->A1) );
            adjust( minx, miny, maxx, maxy, *(i->A2) );
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

