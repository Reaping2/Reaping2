#pragma once
#ifndef INCLUDED_GRID_UIMODEL_H
#define INCLUDED_GRID_UIMODEL_H

#include "uimodel.h"

namespace render {

class GridUiModel : public UiModel
{
public:
    virtual void CollectVertices( Widget const& Wdg, UiVertexInserter_t& Inserter ) const;
};

class GridElemUiModel : public UiModel
{
public:
    virtual void CollectVertices( Widget const& Wdg, UiVertexInserter_t& Inserter ) const;
};

} // namespace render

#endif // INCLUDED_GRID_UIMODEL_H

