#pragma once
#ifndef INCLUDED_IMAGE_UIMODEL_H
#define INCLUDED_IMAGE_UIMODEL_H

#include "uimodel.h"

namespace render {

class ImageUiModel : public UiModel
{
public:
    virtual void CollectVertices( Widget const& Wdg, UiVertexInserter_t& Inserter ) const;
};

} // namespace render

#endif // INCLUDED_IMAGE_UIMODEL_H

