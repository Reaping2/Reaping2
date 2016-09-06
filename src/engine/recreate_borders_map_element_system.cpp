#include "platform/i_platform.h"
#include "recreate_borders_map_element_system.h"
#include "core/map/recreate_borders_map_element.h"
#include "core/i_border_component.h"
#include "core/i_position_component.h"
#include "core/i_collision_component.h"

namespace map {

RecreateBordersMapElementSystem::RecreateBordersMapElementSystem()
    : MapElementSystem()
    , mScene( Scene::Get() )
    , mBorderType( BorderType::Get() )
{
}


Neighbors RecreateBordersMapElementSystem::GetNeighbors( Actor& actor )
{
    Neighbors r;
    auto positionC = actor.Get<IPositionComponent>();
    BOOST_ASSERT( positionC.IsValid() );
    auto collisionC = actor.Get<ICollisionComponent>();
    BOOST_ASSERT( collisionC.IsValid() );
    glm::vec2 posOnMatrix = glm::vec2( positionC->GetX(), positionC->GetY() );
    double radius = collisionC->GetRadius();
    for (BorderType::NeighborDirs_t::const_iterator i = mBorderType.GetNeighborDirs().begin(), e = mBorderType.GetNeighborDirs().end(); i != e; ++i)
    {
        glm::vec2 pos = *i;
        pos = glm::vec2( pos.x * radius * 2, pos.y * radius * 2 ) + posOnMatrix;

        bool foundNeighbor = false;
        int32_t x = pos.x;
        int32_t y = pos.y;
        auto actors = GetActors( x, y );
        if (actors.IsValid())
        {
            for (auto other : *actors)
            {
                if (actor.GetId() == other->GetId()) //TODO there need to be a way to ensure that the neighbor is the same type
                {
                    r.mNeighbors.push_back( Neighbor( other->GetGUID() ) );
                    foundNeighbor = true;
                    break;
                }
            } //search actor for the neighbor pos ended
        }
        if (!foundNeighbor)
        {
            r.mNeighbors.push_back( Neighbor( -1 ) );
        }
    }
    return r;
}

void RecreateBordersMapElementSystem::Init()
{
    MapElementSystem::Init();
}


void RecreateBordersMapElementSystem::Update(double DeltaTime)
{
    MapElementSystem::Update( DeltaTime );
    if (!mActors.empty())
    {
        for (auto actor : mScene.GetActors())
        {
            auto borderC = actor->Get<IBorderComponent>();
            if (borderC.IsValid())
            {
                borderC->SetChanged( false );
            }
        }
        mActors.clear();
        mGrid.clear();
    }
    for( Opt<RecreateBordersMapElement> recreateBordersMapElement : MapElementListFilter<MapSystem::All>( mMapSystem->GetMapElementList(), RecreateBordersMapElement::GetType_static() ) )
    {
        if (recreateBordersMapElement->GetValueId( RecreateBordersMapElement::RecreateNodeId() ) > 0)
        {
            CreateGrid();
            for (auto actor : mActors)
            {
                auto borderC = actor->Get<IBorderComponent>();
                auto neighbors = GetNeighbors( *actor );
                auto borders = neighbors.GetBorders( Neighbors::GetNeighborDirs() );
                auto outerBorders = neighbors.GetBorders( Neighbors::GetNeighborOuterDirs() );
                borderC->SetBorders( borders );
                borderC->SetOuterBorders( outerBorders );
                borderC->SetChanged( true );
            }
        }
        recreateBordersMapElement->ResetValues();
    }
}

void RecreateBordersMapElementSystem::CreateGrid()
{
    mGrid.clear();
    mActors.clear();
    for (auto actor : mScene.GetActors())
    {
        auto borderC = actor->Get<IBorderComponent>();
        auto positionC = actor->Get<IPositionComponent>();
        if (borderC.IsValid()&&positionC.IsValid())
        {
            int32_t x = positionC->GetX();
            int32_t y = positionC->GetY();
            mActors.push_back( actor );
            auto actors = GetActors( x, y, true );
            actors->push_back( actor );
        }
    }

}

Opt<RecreateBordersMapElementSystem::Actors_t> RecreateBordersMapElementSystem::GetActors( int32_t x, int32_t y, bool forceEmplace /*= false*/ )
{
    auto xi = mGrid.find( x );
    if (xi != mGrid.end())
    {
        auto acti = xi->second.find( y );
        if (acti != xi->second.end())
        {
            return &acti->second;
        }
        else if (forceEmplace)
        {
            xi->second.emplace( y, Actors_t() );
            return GetActors( x, y );
        }
    }
    else if (forceEmplace)
    {
        std::map<int32_t, Actors_t> m;
        m.emplace( y, Actors_t() );
        mGrid.emplace( x, m );
        return GetActors( x, y );
    }
    return nullptr;
}

} // namespace map

