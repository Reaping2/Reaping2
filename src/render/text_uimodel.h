#ifndef INCLUDED_RENDER_TEXT_UIMODEL_H
#define INCLUDED_RENDER_TEXT_UIMODEL_H
#include "platform/i_platform.h"
#include "ui/i_ui.h"
#include "uimodel.h"
#include "ui_renderer.h"

struct Text;

class TextUiModel : public UiModel
{
    virtual void CollectVertices( Widget const& Wdg, UiVertexInserter_t& Inserter )const;
    static bool CalcRequiredSize( Widget const& Wdg, glm::vec2& OutReqSize, std::string& OutBuf, float Ratio );
public:
    static void CollectVertices( Text const& text, UiVertexInserter_t& Inserter );
    static bool CalcRequiredSize( Text const& text, glm::vec2& OutReqSize, std::string& OutBuf, float Ratio );
};

#endif//INCLUDED_RENDER_TEXT_UIMODEL_H
