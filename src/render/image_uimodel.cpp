#include "image_uimodel.h"

#include "renderable_repo.h"

namespace render {

void ImageUiModel::CollectVertices( Widget const& Wdg, UiVertexInserter_t& Inserter ) const
{
    int32_t const ActorVisual = Wdg( Widget::PT_ActorVisual );
    if( ActorVisual == 0 )
    {
        UiModel::CollectVertices( Wdg, Inserter );
        return;
    }
    glm::vec4 const& Dim = Wdg.GetDimensions();
    glm::vec4 const& Col = GetColor( Wdg );
    int32_t const Animation = Wdg( Widget::PT_Animation );
    int32_t const state = Wdg( Widget::PT_State );

    static RenderableRepo& renderables = RenderableRepo::Get();

    SpriteCollection const& Sprites = renderables( ActorVisual );
    Sprite const& Spr = Sprites( Animation );
    SpritePhase const& Phase = Spr( state );

    TexturedBox( Dim, Phase, Col, Inserter );
}

}
