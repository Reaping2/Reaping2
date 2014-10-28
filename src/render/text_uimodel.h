#ifndef INCLUDED_RENDER_TEXT_UIMODEL_H
#define INCLUDED_RENDER_TEXT_UIMODEL_H

class TextUiModel : public UiModel
{
    virtual void CollectVertices( Widget const& Wdg, UiVertexInserter_t& Inserter )const;
    static bool CalcRequiredSize( Widget const& Wdg, glm::vec2& OutReqSize, std::string& OutBuf );
};

#endif//INCLUDED_RENDER_TEXT_UIMODEL_H
