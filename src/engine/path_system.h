#ifndef INCLUDED_ENGINE_PATH_SYSTEM_H
#define INCLUDED_ENGINE_PATH_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/map/level_generator/level_generated_event.h"
#include "input/keyboard.h"
#include <unordered_map>
#include <unordered_set>
#include "core/perf_timer.h"
#include "core/actor_event.h"

namespace engine {
namespace path {

struct Cell
{
    bool mFilled = false;
    Cell() = default;
    Cell( bool filled );
};
typedef std::vector<Cell> Grid_t; // a matrix represented by a vector


struct Box
{
    int32_t mX = -1;
    int32_t mY = -1;
    int32_t mZ = -1;
    int32_t mW = -1;
    Box() = default;
    Box( int32_t x, int32_t y, int32_t z, int32_t w );
    // graph coordinates
    glm::vec2 GetCenter() const;

};
struct Graph
{
    typedef std::vector<Box> Nodes_t;
    Nodes_t mNodes;
    typedef std::vector<std::vector<int32_t>> Neighbours_t;
    // node indexes of current nodes neighbours
    Neighbours_t mNeighbours;
    typedef std::unordered_map<int32_t,std::unordered_map<int32_t,double>> DistanceMatrix_t;
    // distance between to nodes in graph coordinates
    DistanceMatrix_t mDistanceMatrix;
    Grid_t mGrid;
    int32_t mGridSize = 0;
    int32_t mCellSize = 0;
    // scene coordinates
    glm::vec2 GetBoxCenter( int32_t ind ) const;
    void GenerateGraph(Grid_t const& grid, int32_t gridSize, int32_t cellSize );
    void CreateQuadNodes();
    Box CreateQuadBox( int32_t x, int32_t y, Grid_t& visited );
    int32_t GetBoxIndex( Actor& actor ) const;
};

struct ActorPathDesc
{
    typedef std::vector<double> NodeDistance_t;
    NodeDistance_t mNodeDistance;
    // the box node where the actor was in the previous frame
    int32_t mPrevNodeIndex = -1;
    typedef std::vector<int32_t> NodeToPrevNode_t;
    NodeToPrevNode_t mNodeToPrevNode;
};

class PathSystem : public System
{
public:

    DEFINE_SYSTEM_BASE(PathSystem)
    PathSystem();
    engine::path::Graph& GetGraph();
    // get distance based on graph in scene coordinates
    double GetDistance( Actor& actor, Actor& targetActor ) const;
    // get direction towards the targetActor on the shortest route
    double GetDirection( Actor& actor, Actor& targetActor ) const;
    double GetBoxIndex( Actor& actor ) const;
    // currently used for debug purpose only
    std::vector<double> const& GetNodeDistance( int32_t actorGUID ) const; 
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
    void LogCells();
private:
    bool mDebugPathSystem = false;
    perf::Timer_t mPerfTimer;
    Scene& mScene;
    AutoReg mOnLevelGenerated;
    void OnLevelGenerated(map::LevelGeneratedEvent const& Evt);
    void CreateGrid();

    // the maximum size of the grid matrix
    int32_t mGridSize;
    // size of a grid cell
    int32_t mCellSize;
    Grid_t mGrid;
    Graph mGraph;
    bool mIsLevelGenerated;
    bool mGenerateNodeDistance;

    void AddDebugBoxes();
    void ShowDebugNeighbours( int32_t neighInd );
    bool GenerateNodeDistance( Actor& actor );
    typedef std::map<int32_t, ActorPathDesc> ActorPathDescs_t;
    ActorPathDescs_t mActorPathDescs;
    AutoReg mOnActor;
    void OnActor( ::ActorEvent const& Evt );
    glm::vec2 DistancePtLine( glm::vec2 start, glm::vec2 end, glm::vec2 point ) const;
    typedef std::map<int32_t, int32_t> IndexToDebugCells_t;
    IndexToDebugCells_t mIndexToDebugCells;
    Opt<engine::KeyboardSystem> mKeyboard;
};

} // namespace path
} // namespace engine

#endif//INCLUDED_ENGINE_PATH_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "path_system" -e "map-levelGenerated"
