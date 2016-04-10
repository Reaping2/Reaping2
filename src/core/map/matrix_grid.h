#ifndef INCLUDED_CORE_MAP_MATRIX_GRID_H
#define INCLUDED_CORE_MAP_MATRIX_GRID_H

#include "i_grid.h"
#include "core/scene.h"
#include "../border_type.h"

namespace map {

class MatrixGrid: public IGrid
{
protected:
    glm::vec2 GetPositionOnMatrix( glm::vec2 position );
public:
    MatrixGrid( int32_t Id );
    virtual void Update( double DeltaTime );
    virtual void SetMousePosition( double x, double y );

    virtual Neighbors GetNeighbors( glm::vec2 position, int32_t actorID );
private:
    Scene& mScene;
    BorderType& mBorderType;
};



} // namespace map
#endif//INCLUDED_CORE_MAP_MATRIX_GRID_H
