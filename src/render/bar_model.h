#ifndef INCLUDED_RENDER_BAR_MODEL_H
#define INCLUDED_RENDER_BAR_MODEL_H
#include "platform/i_platform.h"
#include "ui/i_ui.h"
#include "ui_renderer.h"
#include "uimodel.h"

class BarModel : public UiModel
{
    virtual void CollectVertices( const Widget& Wdg, UiVertexInserter_t& Inserter )const;
};

#endif//INCLUDED_RENDER_BAR_MODEL_H
