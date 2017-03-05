#include "platform/i_platform.h"
#include "path_system.h"
#include "core/i_static_actor_component.h"
#include "core/i_position_component.h"
#include "core/i_collision_component.h"
#include "core/actor_factory.h"
#include "render/i_visual_box_multiplier_component.h"
#include "core/i_renderable_component.h"
#include "engine.h"
#include "core/i_move_component.h"
#include <queue>
#include "platform/settings.h"

namespace engine {
namespace path {

PathSystem::PathSystem()
    : mScene( Scene::Get() )
    , mGridSize(0)
    , mCellSize(25)
    , mIsLevelGenerated(false)
    , mGenerateNodeDistance(false)
{
}


Graph& PathSystem::GetGraph()
{
    return mGraph;
}


double PathSystem::GetDistance( Actor& actor, Actor& targetActor ) const
{
    auto targetPathDesc = mActorPathDescs.find( targetActor.GetGUID() );
    if (targetPathDesc == mActorPathDescs.end())
    {
        return std::numeric_limits<double>::max();
    }
    auto const actorIndex=mGraph.GetBoxIndex( actor );

    return (actorIndex==-1)?std::numeric_limits<double>::max():targetPathDesc->second.mNodeDistance[mGraph.GetBoxIndex( actor )]*mCellSize;
}


double PathSystem::GetDirection( Actor& actor, Actor& targetActor ) const
{
    auto targetPathDesc = mActorPathDescs.find( targetActor.GetGUID() );
    if (targetPathDesc == mActorPathDescs.end())
    {
        return 0.0;
    }
    auto const& actorBoxIndex = mGraph.GetBoxIndex( actor );
    if (actorBoxIndex == -1)
    {
        return 0.0;
    }
    auto const& targetBoxIndex = targetPathDesc->second.mNodeToPrevNode[actorBoxIndex];
    if (targetBoxIndex == -1)
    {
        return 0.0;
    }
    auto positionC(actor.Get<IPositionComponent>());
    // direction is based on the actor position(point) 
    // the actor's box node (start)
    // the target box node (end) the box that's a neighbour to the actor's box and the closest route towards the target
    glm::vec2 const point = glm::vec2(positionC->GetX(),positionC->GetY());
    glm::vec2 const end = mGraph.GetBoxCenter( targetBoxIndex );
    glm::vec2 const start = mGraph.GetBoxCenter( actorBoxIndex );

    glm::vec2 const sp( point - start );
    glm::vec2 const se( end - start );

    auto const distanceV = DistancePtLine( start, end, point );
    auto const distance = std::sqrt( glm::dot( distanceV, distanceV ) );
    double distLimit = 0;
    auto collisionC( actor.Get<ICollisionComponent>() );
    if (collisionC.IsValid())
    {
        distLimit = collisionC->GetRadius()*1.1;
    }
    // the basic idea is to walk towards the center of the box if the angle is greater than 90 degrees
    // if it's less then go towards the line between start and end (to avoid stuck)
    // if the actor is close enough to these targets then it moves towards the target end point.
    if (distance > distLimit )
    {
        double const spAngle = atan2( sp.y, sp.x );
        double const seAngle = atan2( se.y, se.x );
        if (std::abs( spAngle - seAngle ) > boost::math::constants::pi<double>() / 2)
        {
            L2( "towards the start\n" );
            glm::vec2 const ps( start - point );
            return atan2( ps.y, ps.x );
        }
        L2( "towards the line\n" );
        return atan2( distanceV.y, distanceV.x );
    }
    L2( "towards the end\n" );
    glm::vec2 const pe( end - point );
    return atan2( pe.y, pe.x );
}


double PathSystem::GetBoxIndex( Actor& actor ) const
{
    return mGraph.GetBoxIndex( actor );
}

void PathSystem::Init()
{
    mDebugPathSystem = Settings::Get().GetInt( "find_path.debug", 0 ) != 0;
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        auto collisionC( actor.Get<ICollisionComponent>() );
        return actor.Get<IStaticActorComponent>().IsValid()
            && actor.Get<IPositionComponent>().IsValid()
            && collisionC.IsValid()&&collisionC->GetCollisionClass()!=CollisionClass::No_Collision; } );
    mOnLevelGenerated=EventServer<map::LevelGeneratedEvent>::Get().Subscribe( boost::bind( &PathSystem::OnLevelGenerated, this, _1 ) );
    mKeyboard = ::engine::Engine::Get().GetSystem<engine::KeyboardSystem>();
    mOnActor = EventServer<::ActorEvent>::Get().Subscribe( boost::bind( &PathSystem::OnActor, this, _1 ) );
}


void PathSystem::Update(double DeltaTime)
{
    static const auto mDebugNeighbours = Settings::Get().GetInt( "find_path.debug_neighbours", 0 ) != 0;
    static const auto enablePathSystem = Settings::Get().GetInt( "find_path.enable", 1 ) != 0;
    if( !enablePathSystem )
    {
        return;
    }
    static auto& programState( core::ProgramState::Get() );
    if (mDebugNeighbours)
    {
        ShowDebugNeighbours( programState.mControlledActorGUID );
    }
    if (mGenerateNodeDistance)
    {
        mPerfTimer.Log( "pre FindPath" );
        for (auto& clientData : programState.mClientDatas)
        {
            auto actor(mScene.GetActor( clientData.mClientActorGUID ));
            if (actor.IsValid())
            {
                GenerateNodeDistance( *actor );
            }
        }
        
        mPerfTimer.Log( "post FindPath" );
    }

}

bool PathSystem::GenerateNodeDistance( Actor& actor )
{
    static const auto mMaxLength = Settings::Get().GetInt( "find_path.max_path_length", 4000 )/mCellSize;
    int32_t startInd = mGraph.GetBoxIndex( actor );
    if (startInd == -1)
    {
        L2( "Return no Start ind.\n" );
        return false;
    }
    auto& actorPathDesc = mActorPathDescs[actor.GetGUID()];
    if (actorPathDesc.mPrevNodeIndex == startInd)
    {
        // the distance map will not change, the actor is on the same box
        L2( "Return same index.\n" );
        return false;
    }
    actorPathDesc.mPrevNodeIndex = startInd;
    auto& nodeDist = actorPathDesc.mNodeDistance;
    std::set<int32_t> nodes;
    auto& nodeToPrevNode = actorPathDesc.mNodeToPrevNode;
    nodeToPrevNode.clear();
    nodeToPrevNode.resize( mGraph.mNodes.size(), -1 );
    nodeDist.clear();
    nodeDist.resize( mGraph.mNodes.size(), std::numeric_limits<double>::max() );

    nodes.insert( startInd );
    nodeDist[startInd] = 0.0;

    int32_t currInd = startInd;
    double minLength = 0;
    while (minLength<mMaxLength&&!nodes.empty())
    {
        nodes.erase( currInd );
        const double len = nodeDist[currInd];
        auto& distMatCurr = mGraph.mDistanceMatrix[currInd];
        for (auto neigh : mGraph.mNeighbours[currInd])
        {
            const double sumLen = len + distMatCurr[neigh];
            if (nodeDist[neigh]>sumLen)
            {
                nodes.insert( neigh );
                nodeDist[neigh] = sumLen;
                nodeToPrevNode[neigh] = currInd;
            }
        }
        minLength = std::numeric_limits<double>::max();
        for (auto& node : nodes)
        {
            const double len = nodeDist[node];
            if (len < minLength)
            {
                minLength = len;
                currInd = node;
            }
        }
    }
    return true;
}


std::vector<double> const& PathSystem::GetNodeDistance( int32_t actorGUID ) const
{
    auto const& r = mActorPathDescs.find( actorGUID );
    static auto const defaultR = std::vector<double>();
    return r!=mActorPathDescs.end()?r->second.mNodeDistance:defaultR;
}

void PathSystem::OnActor( ::ActorEvent const& Evt )
{
    if (Evt.mState == ActorEvent::Removed)
    {
        mActorPathDescs.erase( Evt.mActor->GetGUID() );
    }
}


glm::vec2 PathSystem::DistancePtLine( glm::vec2 start, glm::vec2 end, glm::vec2 point ) const
{
    auto const se = end - start;
    auto const ps = start - point;
    auto const c = se*(glm::dot( ps, se ) / glm::dot( se, se ));
    auto const d = ps - c;
    return d;
}

void PathSystem::LogCells()
{
    static const auto mDebugLogCells = Settings::Get().GetInt( "find_path.debug_log_cell_size", 300 );
    for (int32_t y = mDebugLogCells; y >= 0; --y)
    {
        for (int32_t x = 0; x < mDebugLogCells; ++x)
        {
            L2( "%d", mGrid[y*mGridSize + x].mFilled );
        }
        L2( "\n" );
    }
}

void PathSystem::OnLevelGenerated( map::LevelGeneratedEvent const& Evt )
{
    if (Evt.mState == map::LevelGeneratedEvent::ActorsSpawned)
    {
        mGenerateNodeDistance = true;
        L2( "Path Level Generated\n" );
        CreateGrid();
        mGraph.GenerateGraph( mGrid, mGridSize, mCellSize );

        if (mDebugPathSystem)
        {
            AddDebugBoxes();
            static const auto mDebugLogCells = Settings::Get().GetInt( "find_path.debug_log_cells", 0 ) != 0;
            if (mDebugLogCells)
            {
                LogCells();
            }
        }
    }
}


void PathSystem::CreateGrid()
{
    const glm::vec4 dimensions = mScene.GetDimensions();
    mGridSize = std::ceil( std::max(
        std::abs( dimensions.x ) + std::abs( dimensions.z ),
        std::abs( dimensions.y ) + std::abs( dimensions.w ) )
        / mCellSize );
    mGrid.clear();
    mGrid.resize( mGridSize*mGridSize );
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
        auto positionC( actor->Get<IPositionComponent>() );
        auto collisionC( actor->Get<ICollisionComponent>() );
        int32_t posX = (std::abs( dimensions.x ) + positionC->GetX() - collisionC->GetRadius()) / mCellSize;
        int32_t posY = (std::abs( dimensions.y ) + positionC->GetY() - collisionC->GetRadius()) / mCellSize;
        int32_t posZ = std::ceil( (std::abs( dimensions.x ) + positionC->GetX() + collisionC->GetRadius()) / mCellSize );
        int32_t posW = std::ceil( (std::abs( dimensions.y ) + positionC->GetY() + collisionC->GetRadius()) / mCellSize );
        for (int32_t y = posY; y < posW; ++y)
        {
            for (int32_t x = posX; x < posZ; ++x)
            {
                mGrid[y*mGridSize + x].mFilled = true;
            }
        }
    }
}

Cell::Cell( bool filled )
    : mFilled( filled )
{

}


Box::Box( int32_t x, int32_t y, int32_t z, int32_t w )
    : mX( x )
    , mY( y )
    , mZ( z )
    , mW( w )
{

}


glm::vec2 Box::GetCenter() const
{
    return glm::vec2( (mZ + mX + 1) / 2.0, (mY + mW + 1) / 2.0 );
}


glm::vec2 Graph::GetBoxCenter( int32_t ind ) const
{
    static auto dimensions = glm::vec2(Scene::Get().GetDimensions());
    return mNodes.at(ind).GetCenter()*mCellSize+dimensions;
}

void Graph::GenerateGraph( Grid_t const& grid, int32_t gridSize, int32_t cellSize )
{
    mGrid = grid;
    mGridSize = gridSize;
    mCellSize = cellSize;
    CreateQuadNodes();
    CreateNodeIndexForCells();

    CreateNeighbours();
    CreateDistanceMatrix();
}

void Graph::CreateDistanceMatrix()
{
    mDistanceMatrix.clear();

    for (int32_t i = 0; i < mNeighbours.size(); ++i)
    {
        auto const& box = mNodes[i];
        auto const& neighbours = mNeighbours[i];
        for (auto ni : neighbours)
        {
            auto const& neigh = mNodes[ni];
            mDistanceMatrix[i].emplace( ni, glm::distance( box.GetCenter(), neigh.GetCenter() ) );
        }
    }
}


void Graph::CreateNodeIndexForCells()
{
    for (int32_t i = 0; i < mNodes.size(); ++i)
    {
        auto const& box = mNodes[i];
        for (int32_t y = box.mY; y >= box.mW; --y)
        {
            int ind = y*mGridSize;
            for (int32_t x = box.mX; x <= box.mZ; ++x)
            {
                mGrid[ind + x].mNodeIndex = i;
            }
        }
    }
}

void Graph::CreateNeighbours()
{
    mNeighbours.clear();
    mNeighbours.resize( mNodes.size() );
    for (int32_t i = 0; i < mNodes.size(); ++i)
    {
        auto const& box = mNodes[i];
        for (int32_t ni = 0; ni < mNodes.size(); ++ni)
        {
            if (ni == i)
            {
                continue;
            }
            auto const& neigh = mNodes[ni];
            if (((box.mY + 1 == neigh.mW //neigh is over
                || box.mW - 1 == neigh.mY) // neigh is under
                && box.mX <= neigh.mZ&&neigh.mX <= box.mZ)
                || ((box.mX - 1 == neigh.mZ //neigh is on the left
                    || box.mZ + 1 == neigh.mX) //neigh is on the right
                    && box.mW <= neigh.mY&&neigh.mW <= box.mY)) // y interval overlap
            {
                mNeighbours[i].push_back( ni );
            }
        }
    }
}

void Graph::CreateQuadNodes()
{
    mNodes.clear();
    Grid_t visited( mGrid.size(), false );
    int32_t ind = 0;
    for (int32_t y = 0; y < mGridSize; ++y)
    {
        for (int32_t x = 0; x < mGridSize; ++x)
        {
            if (!visited[ind].mFilled&&!mGrid[ind].mFilled)
            {
                mNodes.push_back( CreateQuadBox( x, y, visited ) );
            }
            visited[ind].mFilled = true;
            ++ind;
        }
    }
}

void PathSystem::AddDebugBoxes()
{
    auto& actorFactory( ActorFactory::Get() );
    int32_t cellId = AutoId( "cell_debug" );
    auto& scene = Scene::Get();
    mIndexToDebugCells.clear();
    int32_t i = 0;
    for (auto& node : mGraph.mNodes)
    {
        auto cell = actorFactory( cellId );
        auto positionC( cell->Get<IPositionComponent>() );

        positionC->SetX(
            ((node.mZ + node.mX + 1) / 2.0 - mGridSize / 2.0) * mCellSize
            );
        positionC->SetY(
            ((node.mW + node.mY + 1) / 2.0 - mGridSize / 2.0) * mCellSize
            );
        auto visualBoxMC( cell->Get<render::IVisualBoxMultiplierComponent>() );
        visualBoxMC->SetWidth(
            (node.mZ - node.mX + 1) / 2.0
            );
        visualBoxMC->SetHeight(
            (node.mY - node.mW + 1) / 2.0
            );
        mIndexToDebugCells.emplace( i, cell->GetGUID() );
        scene.AddActor( cell.release() );
        ++i;
    }

}

void PathSystem::ShowDebugNeighbours( int32_t neighInd )
{
    auto& scene = Scene::Get();
    for (int32_t i = 0; i < mGraph.mNodes.size(); ++i)
    {
        auto neigh( scene.GetActor( mIndexToDebugCells[i] ) );
        auto renderableC( neigh->Get<IRenderableComponent>() );
        renderableC->SetColor( glm::vec4( 1, 1, 1, 1 ) );
    }
    if (neighInd >= mGraph.mNodes.size() || neighInd < 0)
    {
        return;
    }
    auto act( scene.GetActor( mIndexToDebugCells[neighInd] ) );
    auto renderableC( act->Get<IRenderableComponent>() );
    renderableC->SetColor( glm::vec4( 0, 1, 0, 1 ) );
    for (auto node : mGraph.mNeighbours[neighInd])
    {
        auto neigh( scene.GetActor( mIndexToDebugCells[node] ) );
        auto renderableC( neigh->Get<IRenderableComponent>() );
        renderableC->SetColor( glm::vec4( 1, 0, 0, 1 ) );
    }
}

Box Graph::CreateQuadBox( int32_t x, int32_t y, Grid_t& visited )
{
    static const auto mMaxBoxSize = Settings::Get().GetInt( "find_path.max_box_size", 12 );
    bool allEmpty = true;
    int32_t nextIter = 1;
    while (allEmpty && nextIter <= mMaxBoxSize)
    {
        if (y + nextIter >= mGridSize || x + nextIter >= mGridSize )
        {
            allEmpty = false;
            break;
        }
        int32_t iy = y + nextIter;
        int32_t ix = x;
        for (; ix <= x + nextIter; ++ix)
        {
            const int32_t ind = iy*mGridSize + ix;
            if (mGrid[ind].mFilled||visited[ind].mFilled)
            {
                allEmpty = false;
                break;
            }
        }
        if (!allEmpty)
        {
            break;
        }
        ix = x + nextIter;
        for (iy = y; iy <= y + nextIter; ++iy)
        {
            const int32_t ind = iy*mGridSize + ix;
            if (mGrid[ind].mFilled || visited[ind].mFilled)
            {
                allEmpty = false;
                break;
            }
        }
        if (!allEmpty)
        {
            break;
        }
        ++nextIter;
    }
    --nextIter;
    for (int32_t iy = y; iy <= y + nextIter; ++iy)
    {
        int32_t ind = iy*mGridSize+x;
        for (int32_t ix = x; ix <= x + nextIter; ++ix)
        {
            visited[ind].mFilled = true;
            ++ind;
        }
    }
    return Box( x, y + nextIter,x+nextIter, y );
}

int32_t Graph::GetBoxIndex( Actor& actor ) const
{
    auto positionC( actor.Get<IPositionComponent>() );
    int32_t const x = positionC->GetX() / mCellSize + mGridSize / 2;
    int32_t const y = positionC->GetY() / mCellSize + mGridSize / 2;
    int32_t const ind = y*mGridSize + x;
    return 0<=ind&&ind<mGridSize*mGridSize?mGrid[ind].mNodeIndex:-1;
}


} // namespace path
} // namespace engine

