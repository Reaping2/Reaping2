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
    if (!positionC.IsValid())
    {
        return r;
    }
    auto collisionC = actor.Get<ICollisionComponent>();
    if (!collisionC.IsValid())
    {
        return r;
    }
    glm::vec2 posOnMatrix = glm::vec2( positionC->GetX(), positionC->GetY() );
    double radius = collisionC->GetRadius();
    for (BorderType::NeighborDirs_t::const_iterator i = mBorderType.GetNeighborDirs().begin(), e = mBorderType.GetNeighborDirs().end(); i != e; ++i)
    {
        glm::vec2 pos = *i;
        pos = glm::vec2( pos.x * radius * 2, pos.y * radius * 2 ) + posOnMatrix;

        bool foundNeighbor = false;
        for (auto other : mScene.GetActors())
        {
            Opt<IPositionComponent> positionC = other->Get<IPositionComponent>();
            if (!positionC.IsValid())
            {
                continue;
            }
            if (actor.GetGUID() != other->GetGUID()
                && std::abs( positionC->GetX() - pos.x ) < 0.01
                && std::abs( positionC->GetY() - pos.y ) < 0.01)
            {
                if (actor.GetId() == other->GetId()) //TODO there need to be a way to ensure that the neighbor is the same type
                {
                    r.mNeighbors.push_back( Neighbor( other->GetGUID() ) );
                    foundNeighbor = true;
                }
            }
        } //search actor for the neighbor pos ended
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
    for( Opt<RecreateBordersMapElement> recreateBordersMapElement : MapElementListFilter<MapSystem::All>( mMapSystem->GetMapElementList(), RecreateBordersMapElement::GetType_static() ) )
    {
        if (recreateBordersMapElement->GetValueId( RecreateBordersMapElement::RecreateNodeId() ) > 0)
        {
            for (auto actor : mScene.GetActors())
            {
                auto borderC = actor->Get<IBorderComponent>();
                if (borderC.IsValid())
                {
                    auto neighbors = GetNeighbors( *actor );
                    auto borders = neighbors.GetBorders( Neighbors::GetNeighborDirs() );
                    auto outerBorders = neighbors.GetBorders( Neighbors::GetNeighborOuterDirs() );
                    borderC->SetBorders( borders );
                    borderC->SetOuterBorders( borders );
                }
            }
        }
        recreateBordersMapElement->ResetValues();
    }
}


} // namespace map

