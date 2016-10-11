#ifndef INCLUDED_ENGINE_PATH_SYSTEM_H
#define INCLUDED_ENGINE_PATH_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/map/level_generator/level_generated_event.h"
#include "input/keyboard.h"
#include <unordered_map>
#include <unordered_set>
#include "core/perf_timer.h"

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
    glm::vec2 GetCenter();

};
struct Graph
{
    typedef std::vector<Box> Nodes_t;
    Nodes_t mNodes;
    typedef std::vector<std::vector<int32_t>> Neighbours_t;
    Neighbours_t mNeighbours;
    typedef std::unordered_map<int32_t,std::unordered_map<int32_t,double>> Distance_matrix_t;
    Distance_matrix_t mDistanceMatrix;
    Grid_t mGrid;
    int32_t mGridSize = 0;
    int32_t mCellSize = 0;
    typedef std::vector<double> NodeLength_t;
    NodeLength_t mNodeLength;
    glm::vec2 GetBoxCenter( int32_t ind );
    void GenerateGraph(Grid_t const& grid, int32_t gridSize, int32_t cellSize );

    void CreateFloodNodes();
    void CreateQuadNodes();

    Box CreateBox( int32_t x, int32_t y, Grid_t& visited );
    Box CreateQuadBox( int32_t x, int32_t y, Grid_t& visited );
    int32_t GetBoxIndex( Actor& actor );
};
class PathSystem : public System
{
public:

    DEFINE_SYSTEM_BASE(PathSystem)
    PathSystem();
    engine::path::Graph& GetGraph();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );

    void AddDebugStartPoint();

    void LogCells();

private:
    perf::Timer_t mPerfTimer;
    Scene& mScene;
    AutoReg mOnLevelGenerated;
    void OnLevelGenerated(map::LevelGeneratedEvent const& Evt);

    void CreateGrid();

    int32_t mGridSize;
    int32_t mCellSize;
    Grid_t mGrid;
    Graph mGraph;
    bool mIsLevelGenerated;
    bool mDebugOnPlayerPosition;

    void AddDebugBoxes();
    void ShowDebugPath();
    void ShowDebugNeighbours( int32_t neighInd );
    typedef std::list<int32_t> Path_t;
    Path_t mPath;
    Path_t FindPath( Actor& actorA, Actor& actorB );
    Path_t FindPathDijkstra( Actor& actorA, Actor& actorB );
    typedef std::map<int32_t, int32_t> index_to_debug_cells_t;
    index_to_debug_cells_t mIndexToDebugCells;
    Opt<engine::KeyboardSystem> mKeyboard;
    int32_t mDebugActorGUID = -1;
    int32_t mPrevEndActorInd = -1;
};

} // namespace path
} // namespace engine

#endif//INCLUDED_ENGINE_PATH_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "path_system" -e "map-levelGenerated"
