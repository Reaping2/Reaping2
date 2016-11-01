#ifndef INCLUDED_CORE_GRID_H
#define INCLUDED_CORE_GRID_H
#include "core/actor.h"
#include "collision_class.h"
#include "i_collision_component.h"
#include <map>
#include <set>
#include <vector>

struct CollPair
{
    Actor* A1;
    Actor* A2;
    CollPair( Actor* a1, Actor* a2 );
};
struct CollPairCmp
{
    bool operator ()( CollPair const& a, CollPair const& b )const;
};
typedef std::set<CollPair, CollPairCmp> PossibleCollisions_t;

class Grid
{
public:
    void Build( glm::vec4 const& Dimensions, float CellSize );
    void AddActor( Actor* A, double Dt, Opt<ICollisionComponent> collisionC );
    void RemoveActor( Actor* A );
    PossibleCollisions_t GetPossibleCollisions()const;
private:
    static const uint32_t Collisions[];
    typedef std::vector<Actor*> Actors_t;
    struct Cell
    {
        Actors_t mActors[CollisionClass::Num_Classes];
    };
    typedef std::vector<Cell> Cells_t;
    std::map<Actor*,std::vector<Cell*> > mActorInCell;
    float mCellSize;
    size_t mDimX;
    size_t mDimY;
    glm::vec2 mMin;
    Cells_t mCells;
    glm::vec4 Box( Actor const& Obj, double Dt )const;
};

#endif//INCLUDED_CORE_GRID_H
