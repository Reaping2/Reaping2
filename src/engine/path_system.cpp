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
    , mDebugOnPlayerPosition(false)
{
}


Graph& PathSystem::GetGraph()
{
    return mGraph;
}

void PathSystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IStaticActorComponent>().IsValid()
            && actor.Get<IPositionComponent>().IsValid()
            && actor.Get<ICollisionComponent>().IsValid(); } );
    mOnLevelGenerated=EventServer<map::LevelGeneratedEvent>::Get().Subscribe( boost::bind( &PathSystem::OnLevelGenerated, this, _1 ) );
    mKeyboard = ::engine::Engine::Get().GetSystem<engine::KeyboardSystem>();
}


void PathSystem::Update(double DeltaTime)
{
    static auto& programState( core::ProgramState::Get() );
    if (mDebugOnPlayerPosition)
    {
        mPerfTimer.Log( "pre FindPath" );
        auto actor(mScene.GetActor( programState.mControlledActorGUID ));
        if (actor.IsValid())
        {
            int32_t i = mGraph.GetBoxIndex( *actor );
            if (i != -1)
            {
                //ShowDebugNeighbours( i );
            }
        }
        if (mDebugActorGUID == -1)
        {
            AddDebugStartPoint();
        }
        if (mKeyboard->GetKey( GLFW_KEY_F ).State == KeyState::Typed)
        {
            mScene.RemoveActor( mDebugActorGUID );
            AddDebugStartPoint();
        }
        auto actorB( mScene.GetActor( mDebugActorGUID ) );
        mPath = FindPathDijkstra( *actorB, *actor );
//         ShowDebugPath();
        mPerfTimer.Log( "post FindPath" );
    }

}


void PathSystem::AddDebugStartPoint()
{
    static auto& programState( core::ProgramState::Get() );
    static auto& actorFactory( ActorFactory::Get() );
    auto cloneActor = actorFactory( AutoId( "player" ) );
    auto clonePositionC( cloneActor->Get<IPositionComponent>() );
    auto cloneMoveC( cloneActor->Get<IMoveComponent>() );
    cloneMoveC->SetMoving( false );
    auto actor( mScene.GetActor( programState.mControlledActorGUID ) );
    if (actor.IsValid())
    {
        auto positionC( actor->Get<IPositionComponent>() );
        clonePositionC->SetX( positionC->GetX() );
        clonePositionC->SetY( positionC->GetY() );
    }
    mDebugActorGUID = cloneActor->GetGUID();
    mScene.AddActor( cloneActor.release() );
}

void PathSystem::LogCells()
{
    for (int y = 300; y >= 0; --y)
    {
        for (int x = 0; x < 300; ++x)
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
        mDebugOnPlayerPosition = true;
        L2( "Path Level Generated\n" );
        CreateGrid();
        mGraph.GenerateGraph( mGrid, mGridSize, mCellSize );
        AddDebugBoxes();
        LogCells();
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
    for (auto actor : mScene.GetActors())
    {
        auto staticActorComp( actor->Get<IStaticActorComponent>() );
        if (staticActorComp.IsValid())
        {
            auto positionC( actor->Get<IPositionComponent>() );
            auto collisionC( actor->Get<ICollisionComponent>() );
        }
    }
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


glm::vec2 Box::GetCenter()
{
    return glm::vec2( (mZ + mX + 1) / 2.0, (mY + mW + 1) / 2.0 );
}


glm::vec2 Graph::GetBoxCenter( int32_t ind )
{
    static auto dimensions = glm::vec2(Scene::Get().GetDimensions());
    return mNodes[ind].GetCenter()*mCellSize+dimensions;
}

void Graph::GenerateGraph( Grid_t const& grid, int32_t gridSize, int32_t cellSize )
{
    mGrid = grid;
    mGridSize = gridSize;
    mCellSize = cellSize;
    CreateQuadNodes();

    mNeighbours.clear();
    mNeighbours.resize( mNodes.size() );
    for (int i = 0; i < mNodes.size();++i)
    {
        auto box = mNodes[i];
        for (int ni = 0; ni < mNodes.size(); ++ni)
        {
            if (ni == i)
            {
                continue;
            }
            auto neigh = mNodes[ni];
            if (((box.mY + 1 == neigh.mW //neigh is over
                    || box.mW - 1 == neigh.mY) // neigh is under
                       && box.mX<=neigh.mZ&&neigh.mX<=box.mZ)
                || ((box.mX - 1 == neigh.mZ //neigh is on the left
                    || box.mZ + 1 == neigh.mX) //neigh is on the right
                    && box.mW <= neigh.mY&&neigh.mW <= box.mY)) // y interval overlap
            {
                mNeighbours[i].push_back( ni );
                const double distance = std::sqrt(glm::distance2( 
                    glm::vec2((box.mZ + box.mX + 1) / 2.0, (box.mY + box.mW + 1) / 2.0 ), 
                    glm::vec2((neigh.mZ + neigh.mX + 1) / 2.0, (neigh.mY + neigh.mW + 1) / 2.0 )
                    ));
                mDistanceMatrix[i].emplace( ni, distance );
                if (distance < 100)
                {
                    int h = 7;
                }
            }
        }
    }
}


void Graph::CreateFloodNodes()
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
                mNodes.push_back( CreateBox( x, y, visited ) );
            }
            visited[ind].mFilled = true;
            ++ind;
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

void PathSystem::ShowDebugPath()
{
    auto& scene = Scene::Get();
    for (int i = 0; i < mGraph.mNodes.size(); ++i)
    {
        auto neigh( scene.GetActor( mIndexToDebugCells[i] ) );
        auto renderableC( neigh->Get<IRenderableComponent>() );
        renderableC->SetColor( glm::vec4( 1, 1, 1, 1 ) );
    }
    for (auto node : mPath)
    {
        auto neigh( scene.GetActor( mIndexToDebugCells[node] ) );
        auto renderableC( neigh->Get<IRenderableComponent>() );
        renderableC->SetColor( glm::vec4( 1, 0, 0, 1 ) );
    }
}

void PathSystem::ShowDebugNeighbours( int32_t neighInd )
{
    auto& scene = Scene::Get();
    for (int i = 0; i < mGraph.mNodes.size(); ++i)
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


engine::path::PathSystem::Path_t PathSystem::FindPath( Actor& actorA, Actor& actorB )
{
    int32_t startInd = mGraph.GetBoxIndex( actorA );
    int32_t endInd = mGraph.GetBoxIndex( actorB );
    Path_t r;
    if (startInd == -1 || endInd == -1)
    {
        return r;
    }
    std::deque<int32_t> nodes;
    std::set<int32_t> visited;
    std::map<int32_t, int32_t> destToStartMap;
    nodes.push_back( startInd );
    int32_t currInd = nodes.front();
    while (currInd != endInd && !nodes.empty())
    {
        nodes.pop_front();
        visited.insert( currInd );
        for (auto neigh : mGraph.mNeighbours[currInd])
        {
            if (visited.find( neigh ) == visited.end())
            {
                nodes.push_back( neigh );
                destToStartMap.emplace( neigh, currInd );
            }
        }
        currInd = nodes.front();
    }
    if (currInd == endInd)
    {
        while (currInd != startInd)
        {
            r.push_front( currInd );
            currInd = destToStartMap[currInd];
        }
        r.push_front( currInd );
    }
    
    return r;
}


engine::path::PathSystem::Path_t PathSystem::FindPathDijkstra( Actor& actorA, Actor& actorB )
{
    static const auto mMaxLength = Settings::Get().GetInt( "find_path.max", 400 );
    int32_t startInd = mGraph.GetBoxIndex( actorA );
    int32_t endInd = mGraph.GetBoxIndex( actorB );
    Path_t r;
    if (startInd == -1 || endInd == -1)
    {
        return r;
    }
    if (mPrevEndActorInd == endInd)
    {
        return r;
    }
    mPrevEndActorInd = endInd;
    std::set<int32_t> nodes;
    std::vector<int32_t> destToStartMap;
    destToStartMap.resize( mGraph.mNodes.size(), -1 );
    mGraph.mNodeLength.clear();
    mGraph.mNodeLength.resize( mGraph.mNodes.size(), std::numeric_limits<double>::max() );

    nodes.insert( startInd );
    mGraph.mNodeLength[startInd]= 0.0;

    int32_t currInd = startInd;
    double currLen = std::numeric_limits<double>::max();
    double minLength = 0;
    bool done = false;
    //bool foundEnd = false;
    while (minLength<mMaxLength&&!nodes.empty())
    {
        nodes.erase(currInd);
        const double len= mGraph.mNodeLength[currInd];
        auto& distMatCurr = mGraph.mDistanceMatrix[currInd];
        for (auto neigh : mGraph.mNeighbours[currInd])
        {
            const double sumLen = len + distMatCurr[neigh];
            if (mGraph.mNodeLength[neigh]>sumLen)
            {
                nodes.insert( neigh );
                mGraph.mNodeLength[neigh] = sumLen;
                destToStartMap[neigh] = currInd;
            }
        }
        minLength = std::numeric_limits<double>::max();
        for (auto& node : nodes)
        {
            const double len = mGraph.mNodeLength[node];
            if (len < minLength)
            {
                minLength = len;
                currInd = node;
            }
        }
        if (currInd == endInd)
        {
            if (minLength < currLen)
            {
                currLen = minLength;
            }
        }
        if (currInd == endInd)
        {
            int32_t minInd = endInd;
            while (minInd != startInd)
            {
                r.push_front( minInd );
                minInd = destToStartMap[minInd];
            }
            r.push_front( minInd );
        }
    }

    return r;
}

Box Graph::CreateBox( int32_t x, int32_t y, Grid_t& visited )
{
    Box box(x, mGridSize, mGridSize, y);
    int32_t ix = x;
    int32_t iy = y;
    int32_t ind = iy*mGridSize + ix;
    while (ix < mGridSize&&!visited[ind].mFilled&&!mGrid[ind].mFilled)
    {
        visited[ind].mFilled = true;
        ++ind;
        ++ix;
    }
    box.mZ = ix-1;
    iy = y + 1;
    while (iy < mGridSize)
    {
        ix = x;
        ind = iy*mGridSize + ix;
        while (ix <= box.mZ&&!visited[ind].mFilled&&!mGrid[ind].mFilled)
        {
            ++ind;
            ++ix;
        }
        if (ix <= box.mZ)
        {
            break;
        }
        ix = x;
        ind = iy*mGridSize + ix;
        for (; ix <= box.mZ; ++ix)
        {
            visited[ind].mFilled = true;
            ++ind;
        }
        ++iy;
    }
    box.mY = iy - 1;
    return box;
}


Box Graph::CreateQuadBox( int32_t x, int32_t y, Grid_t& visited )
{
    bool allEmpty = true;
    int32_t nextIter = 1;
    while (allEmpty)
    {
        if (y + nextIter >= mGridSize || x + nextIter >= mGridSize)
        {
            allEmpty = false;
            --nextIter;
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
    //--nextIter;
    return Box( x, y + nextIter,x+nextIter, y );
}

int32_t Graph::GetBoxIndex( Actor& actor )
{
    auto positionC( actor.Get<IPositionComponent>() );
    int32_t x = positionC->GetX() / mCellSize + mGridSize / 2;
    int32_t y = positionC->GetY() / mCellSize + mGridSize / 2;
    for (int i = 0; i < mNodes.size(); ++i)
    {
        auto box = mNodes[i];
        if (box.mX <= x&&x <= box.mZ
            &&box.mW <= y&&y <= box.mY)
        {
            return i;
        }
    }
    return -1;
}

// struct Coord {
//     int32_t mX = -1;
//     int32_t mY = -1;
//     Coord() = default;
//     Coord( int32_t x, int32_t y ) :mX( x ), mY( y ) {}
// };
// typedef std::deque<Coord> Coords_t;
// Coords_t coords;
// coords.push_back( Coord( x, y ) );
// while (!coords.empty())
// {
//     auto const& coord = coords.front();
//     coords.pop_front();
//     visited[coord.mY*mGridSize + coord.mX].mFilled = true;
//     for (int32_t iy = -1; iy <= 1; ++iy)
//     {
//         for (int32_t ix = -1; ix <= 1; ++ix)
//         {
//             if (iy == 0 && ix == 0)
//             {
//                 continue;
//             }
//             const int32_t y = coord.mY + iy;
//             const int32_t x = coord.mX + ix;
//             if (x > box.mX && x<box.mZ && y>box.mY && y < box.mW)
//             {
//                 const int32_t ind = y*mGridSize + x;
//                 if (visited[ind].mFilled)
//                 {
//                     continue;
//                 }
//                 if (mGrid[ind].mFilled)
//                 {
//                     if (iy < 0)
//                     {
//                         box.mY = std::max( box.mY, y );
//                     }
//                     else if (iy > 0)
//                     {
//                         box.mW = std::min( box.mW, y );
//                     }
//                     if (ix < 0)
//                     {
//                         box.mX = std::max( box.mX, x );
//                     }
//                     else if (ix > 0)
//                     {
//                         box.mZ = std::min( box.mZ, x );
//                     }
//                 }
//                 else
//                 {
//                     coords.push_back( Coord( x, y ) );
//                 }
//                 visited[ind].mFilled = true;
//             }
//         }
//     }
// }


} // namespace path
} // namespace engine

