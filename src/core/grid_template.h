#ifndef INCLUDED_CORE_GRID_TEMPLATE_H
#define INCLUDED_CORE_GRID_TEMPLATE_H
#include "core/actor.h"
#include <map>
#include <set>
#include <vector>
#include <functional>

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

template<typename Traits>
class GridTemplate
{
public:
    void Build( glm::vec4 const& Dimensions, float CellSize );
    void AddActor( Actor* A, double Dt );
    void RemoveActor( Actor* A );
    PossibleCollisions_t const& GetPossibleCollisions()const;
    PossibleCollisions_t GetPossibleCollisions( glm::vec4 const& rect )const;
    typedef std::vector<Actor*> Actors_t;
    typedef std::function<void( typename Actors_t::const_iterator, typename Actors_t::const_iterator )> CollectorFunction;
    void CollectActorsWithMaskAndAround( int32_t collMask, CollectorFunction const& func ) const;
    std::set<Actor*> GetAllNearbyActors( glm::vec2 const& position, double radius, int32_t collMask, glm::vec2 const* direction = nullptr ) const;
    std::set<Actor*> GetAllNearbyActors( Actor const* A ) const;
private:
    struct Cell
    {
        Actors_t mActors[Traits::NumClasses];
        mutable PossibleCollisions_t mPossibleCollisions;
        mutable bool mDirty = true;
        void Refresh() const;
    };
    typedef std::vector<Cell> Cells_t;
    std::map<Actor const*,std::vector<Cell*> > mActorInCell;
    float mCellSize;
    size_t mDimX;
    size_t mDimY;
    glm::vec4 mDims;
    glm::vec2 mMin;
    Cells_t mCells;
    mutable PossibleCollisions_t mPossibleCollisions;
    mutable bool mDirty = true;
    glm::vec4 Box( Actor const& Obj, double Dt )const;
};

#endif//INCLUDED_CORE_GRID_TEMPLATE_H
