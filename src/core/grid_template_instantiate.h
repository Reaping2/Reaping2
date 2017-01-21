#include "platform/i_platform.h"
#include "core/grid_template.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include <utility>

template<typename Traits>
void GridTemplate<Traits>::Cell::Refresh() const
{
    if( !mDirty )
    {
        return;
    }
    mDirty = false;
    mPossibleCollisions.clear();
    for (size_t k = 0; k < Traits::NumClasses; ++k)
    {
        Actors_t const& Actors = mActors[k];
        for( size_t j = k; j < Traits::NumClasses; ++j )
        {
            if( !Traits::AreCorrelated( k, j ) )
            {
                continue;
            }
            Actors_t const& Actors2 = mActors[j];
            for( auto const& a : Actors )
            {
                for( auto const& b : Actors2 )
                {
                    if( a == b )
                    {
                        continue;
                    }
                    mPossibleCollisions.insert( CollPair( a, b ) );
                }
            }
        }
    }
}

template<typename Traits>
PossibleCollisions_t const& GridTemplate<Traits>::GetPossibleCollisions()const
{
    // todo? store dirty flag per-cell, and also partial results
    if( !mDirty )
    {
        return mPossibleCollisions;
    }
    mDirty = false;
    mPossibleCollisions = GetPossibleCollisions( mDims );
    return mPossibleCollisions;
}

template<typename Traits>
PossibleCollisions_t GridTemplate<Traits>::GetPossibleCollisions( glm::vec4 const& rect )const
{
    PossibleCollisions_t PossibleCollisions;

    glm::vec4 const ActorDim = rect - glm::vec4( mMin.x, mMin.y, mMin.x, mMin.y );

    size_t const Ex = ( size_t )glm::floor( std::max<float>( 0.0f, ActorDim.z ) / mCellSize );
    size_t const Ey = ( size_t )glm::floor( std::max<float>( 0.0f, ActorDim.w ) / mCellSize );
    size_t const Sx = ( size_t )glm::floor( std::max<float>( 0.0f, ActorDim.x ) / mCellSize );
    size_t const Sy = ( size_t )glm::floor( std::max<float>( 0.0f, ActorDim.y ) / mCellSize );
    for( size_t y = Sy, ey = std::min<size_t>( Ey + 1, mDimY ); y < ey; ++y )
    {
        for( size_t x = Sx, ex = std::min<size_t>( Ex + 1, mDimX ); x < ex; ++x )
        {
            auto& C= mCells[y * mDimX + x];
            C.Refresh();
            PossibleCollisions.insert( C.mPossibleCollisions.begin(),
                C.mPossibleCollisions.end() );
        }
    }
    return PossibleCollisions;
}

template<typename Traits>
void GridTemplate<Traits>::Build( glm::vec4 const& Dimensions, float CellSize )
{
    mDims = Dimensions;
    mActorInCell.clear();
    mCells.clear();
    mCellSize = CellSize;
    mMin = glm::vec2( Dimensions.x, Dimensions.y );
    if( mCellSize < std::numeric_limits<float>::epsilon() )
    {
        mDimX = mDimY = 0;
        assert( false );
        return;
    }
    float const Width = Dimensions.z - Dimensions.x;
    float const Height = Dimensions.w - Dimensions.y;
    mDimX = ( size_t )glm::ceil( Width / mCellSize );
    mDimY = ( size_t )glm::ceil( Height / mCellSize );
    mCells.resize( mDimX * mDimY );
    mDirty = true;
}

namespace {
template <typename T> int sgn(T val)
{
        return (T(0) < val) - (val < T(0));
}
}

template<typename Traits>
std::set<Actor*> GridTemplate<Traits>::GetAllNearbyActors( glm::vec2 const& position, double radius, int32_t collMask, glm::vec2 const* direction ) const
{
    glm::vec4 ActorDim( position.x - radius - mMin.x, position.y - radius - mMin.y,
            position.x + radius - mMin.x, position.y + radius - mMin.y );

    size_t const Ex = ( size_t )glm::floor( std::max<float>( 0.0f, ActorDim.z ) / mCellSize );
    size_t const Ey = ( size_t )glm::floor( std::max<float>( 0.0f, ActorDim.w ) / mCellSize );
    size_t const Sx = ( size_t )glm::floor( std::max<float>( 0.0f, ActorDim.x ) / mCellSize );
    size_t const Sy = ( size_t )glm::floor( std::max<float>( 0.0f, ActorDim.y ) / mCellSize );

    std::set<Actor*> rv;
    std::set<int> done;

    int dirx( 0 ), diry( 0 );
    glm::vec2 transition;
    if( nullptr != direction && *direction != glm::vec2() )
    {
        dirx = sgn( direction->x );
        diry = sgn( direction->y );
        transition = glm::vec2( direction->x * dirx, direction->y * diry );
        auto m = std::max( transition.x, transition.y );
        if( 0.0 != m )
        {
            transition /= m;
        }
    }

    double dsx( Sx ), dsy( Sy ), dex( Ex ), dey( Ey );
    while( dsx >= 0 && dsy >= 0 && dex <= mDimX && dey <= mDimY )
    {
        for( size_t y = floor( dsy ), ey = std::min<size_t>( ceil( dey ) + 1, mDimY ); y < ey; ++y )
        {
            for( size_t x = floor( dsx ), ex = std::min<size_t>( ceil( dex ) + 1, mDimX ); x < ex; ++x )
            {
                if( !done.insert( y * mDimX + x ).second )
                {
                    continue;
                }
                auto const& cell = mCells[y * mDimX + x];
                for( size_t j = 0; j < Traits::NumClasses; ++j )
                {
                    if( !( collMask & ( 1 << j ) ) )
                    {
                        continue;
                    }
                    rv.insert( cell.mActors[j].begin(), cell.mActors[j].end() );
                }
            }
        }
        if( dirx == 0 && diry == 0 )
        {
            break;
        }
        dsx += transition.x * dirx;
        dsy += transition.y * diry;
        dex += transition.x * dirx;
        dey += transition.y * diry;
    }

    return std::move( rv );
}

template<typename Traits>
std::set<Actor*> GridTemplate<Traits>::GetAllNearbyActors( Actor const* A ) const
{
    std::set<Actor*> rv;
    auto const CC = Traits::GetTrait( *A );
    auto it = mActorInCell.find( A );
    if( it == mActorInCell.end() )
    {
        return std::move( rv );
    }
    auto const& cells = it->second;
    for( auto const& cell : cells )
    {
        for( size_t j = 0; j < Traits::NumClasses; ++j )
        {
            if( !Traits::AreCorrelated( CC, j ) )
            {
                continue;
            }
            auto& actors = cell->mActors[ j ];
            rv.insert( actors.begin(), actors.end() );
        }
    }
    rv.erase( const_cast<Actor*>( A ) );
    return std::move( rv );
}

template<typename Traits>
void GridTemplate<Traits>::AddActor( Actor* A, double Dt )
{
    auto const CC = Traits::GetTrait( *A );
    auto& cells = mActorInCell[ A ];
    for( auto& cell : cells )
    {
        auto& actors = cell->mActors[ CC ];
        actors.erase( std::remove( actors.begin(), actors.end(), A ), actors.end() );
    }
    auto oldcells = std::move( cells );
    size_t const NumCells = mCells.size();
    if( NumCells == 0 )
    {
        assert( false );
        return;
    }
    else if( NumCells == 1 )
    {
        mCells[0].mActors[CC].push_back( A );
        cells.push_back( &mCells[0] );
        return;
    }
    glm::vec4 const& ActorDim = Box( *A, Dt );

    size_t const Ex = ( size_t )glm::floor( std::max<float>( 0.0f, ActorDim.z ) / mCellSize );
    size_t const Ey = ( size_t )glm::floor( std::max<float>( 0.0f, ActorDim.w ) / mCellSize );
    size_t const Sx = ( size_t )glm::floor( std::max<float>( 0.0f, ActorDim.x ) / mCellSize );
    size_t const Sy = ( size_t )glm::floor( std::max<float>( 0.0f, ActorDim.y ) / mCellSize );
    for( size_t y = Sy, ey = std::min<size_t>( Ey + 1, mDimY ); y < ey; ++y )
    {
        for( size_t x = Sx, ex = std::min<size_t>( Ex + 1, mDimX ); x < ex; ++x )
        {
            auto& cell = mCells[y * mDimX + x];
            cell.mActors[CC].push_back( A );
            cells.push_back( &cell );
        }
    }
    bool cellsdirty = oldcells.size() != cells.size() || oldcells != cells;
    if( cellsdirty )
    {
        for( auto& c : cells )
        {
            c->mDirty = true;
        }
        for( auto& c : oldcells )
        {
            c->mDirty = true;
        }
    }
    mDirty = mDirty || cellsdirty;
}

template<typename Traits>
glm::vec4 GridTemplate<Traits>::Box( Actor const& Obj, double Dt )const
{
    Opt<IMoveComponent> moveC = Obj.Get<IMoveComponent>();
    float const MvX = moveC.IsValid() ? Dt * moveC->GetSpeedX() : 0.0f;
    float const MvY = moveC.IsValid() ? Dt * moveC->GetSpeedY() : 0.0f;

    double const Radius = Traits::GetRadius( Obj );
    Opt<IPositionComponent> const objPositionC = Obj.Get<IPositionComponent>();
    double const Ox = objPositionC->GetX() - mMin.x;
    double const Oy = objPositionC->GetY() - mMin.y;
    glm::vec4 Ret( Ox - Radius,
                   Oy - Radius,
                   Ox + Radius,
                   Oy + Radius );
    if( MvX < 0.0 )
    {
        Ret.x += MvX;
    }
    else
    {
        Ret.z += MvX;
    }
    if( MvY < 0.0 )
    {
        Ret.y += MvY;
    }
    else
    {
        Ret.w += MvY;
    }
    return Ret;
}

template<typename Traits>
void GridTemplate<Traits>::RemoveActor( Actor* A )
{
    auto& cells = mActorInCell[ A ];
    for( auto& cell : cells )
    {
        for( auto& actors : cell->mActors )
        {
            actors.erase( std::remove( actors.begin(), actors.end(), A ), actors.end() );
        }
        cell->mDirty = true;
    }
    mActorInCell.erase( A );
    mDirty = true;
}

#define GRID_TEMPLATE_INSTANTIATE( BASE ) \
    template void GridTemplate<BASE>::Build( glm::vec4 const& Dimensions, float CellSize ); \
    template void GridTemplate<BASE>::AddActor( Actor* A, double Dt ); \
    template void GridTemplate<BASE>::RemoveActor( Actor* A ); \
    template PossibleCollisions_t const& GridTemplate<BASE>::GetPossibleCollisions()const; \
    template std::set<Actor*> GridTemplate<BASE>::GetAllNearbyActors( glm::vec2 const& position, double radius, int32_t collMask, glm::vec2 const* direction = nullptr ) const; \
    template std::set<Actor*> GridTemplate<BASE>::GetAllNearbyActors( Actor const* A ) const;

