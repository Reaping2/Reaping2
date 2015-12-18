#include "grid_uimodel.h"
#include "ui/ui_model.h"

namespace render {
void GridUiModel::CollectVertices( Widget const& wdg, UiVertexInserter_t& inserter ) const
{
    wdg( Widget::PT_StartIdBase ).ResolveModel()();
}

void GridElemUiModel::CollectVertices( Widget const& wdg, UiVertexInserter_t& inserter ) const
{
    wdg( Widget::PT_StartIdStep ).ResolveModel()();
    int32_t currentIndex = wdg( Widget::PT_StartId ).operator int32_t();
    int32_t size = wdg( Widget::PT_Source ).ResolveModel().operator std::vector<int32_t>().size();
    bool visible = currentIndex < size;
    const_cast<Widget::Prop&>( wdg( Widget::PT_SubtreeHidden ) ) = visible ? 0 : 1;
}
}

