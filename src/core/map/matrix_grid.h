#ifndef INCLUDED_CORE_MAP_MATRIX_GRID_H
#define INCLUDED_CORE_MAP_MATRIX_GRID_H

#include "i_grid.h"

namespace map {

class MatrixGrid:public IGrid
{
public:
    MatrixGrid(int32_t Id);
    virtual void Update( double DeltaTime );
    virtual void SetMousePosition(double x, double y);
};



} // namespace map
#endif//INCLUDED_CORE_MAP_MATRIX_GRID_H