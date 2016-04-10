#include "matrix_grid.h"
#include "../i_collision_component.h"
#include "../opt.h"
#include "../actor.h"
#include "editor_target_system.h"
#include "../i_position_component.h"

namespace map {

MatrixGrid::MatrixGrid( int32_t Id )
    : IGrid( Id )
    , mScene( Scene::Get() )
    , mBorderType( BorderType::Get() )
{
}


void MatrixGrid::Update( double DeltaTime )
{

}

void MatrixGrid::SetMousePosition( double x, double y )
{
    IGrid::SetMousePosition( x, y );
    mProcessedPosition = mMousePosition;
    mProcessedPosition = GetPositionOnMatrix( mProcessedPosition );
}

Neighbors MatrixGrid::GetNeighbors( glm::vec2 position, int32_t actorID )
{
    Neighbors r;
    glm::vec2 posOnMatrix = GetPositionOnMatrix( position );
    double radius = EditorTargetSystem::Get()->GetCursorRadius();
    for ( BorderType::NeighborDirs_t::const_iterator i = mBorderType.GetNeighborDirs().begin(), e = mBorderType.GetNeighborDirs().end(); i != e; ++i )
    {
        glm::vec2 pos = *i;
        pos = glm::vec2( pos.x * radius * 2, pos.y * radius * 2 ) + posOnMatrix;

        int32_t curGUID = EditorTargetSystem::Get()->GetCursor()->GetGUID();
        bool foundNeighbor = false;
        for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); !foundNeighbor && it != e; ++it )
        {
            Actor& actor = **it;
            Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
            if ( !positionC.IsValid() )
            {
                continue;
            }
            if ( curGUID != actor.GetGUID()
                 && std::abs( positionC->GetX() - pos.x ) < 0.01
                 && std::abs( positionC->GetY() - pos.y ) < 0.01 )
            {
                if( actorID == actor.GetId() ) //TODO there need to be a way to ensure that the neighbor is the same type
                {
                    r.mNeighbors.push_back( Neighbor( actor.GetGUID() ) );
                    foundNeighbor = true;
                }
            }
        } //search actor for the neighbor pos ended
        if ( !foundNeighbor )
        {
            r.mNeighbors.push_back( Neighbor( -1 ) );
        }
    }
    return r;
}

glm::vec2 MatrixGrid::GetPositionOnMatrix( glm::vec2 position )
{
    glm::vec2 r = position;
    double radius = EditorTargetSystem::Get()->GetCursorRadius();
    r.x = std::floor( position.x / ( radius * 2 ) ) * ( radius * 2 ) + std::floor( radius );
    r.y = std::floor( position.y / ( radius * 2 ) ) * ( radius * 2 ) + std::floor( radius );
    return r;
}


} // namespace map

