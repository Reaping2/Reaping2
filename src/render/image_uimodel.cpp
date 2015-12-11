#include "image_uimodel.h"

#include "platform/id_storage.h"
#include "renderable_repo.h"

namespace render {

void ImageUiModel::CollectVertices( Widget const& Wdg, UiVertexInserter_t& Inserter ) const
{
    std::string const& ActorVisual = Wdg( Widget::PT_ActorVisual );
    if( ActorVisual.empty() )
    {
        UiModel::CollectVertices( Wdg, Inserter );
        return;
    }
    glm::vec4 const& Dim = Wdg.GetDimensions();
    glm::vec4 const& Col = GetColor( Wdg );
    std::string const& Animation = Wdg( Widget::PT_Animation );
    int32_t const state = Wdg( Widget::PT_State );

    static platform::IdStorage& ids = platform::IdStorage::Get();
    static RenderableRepo& renderables = RenderableRepo::Get();

    SpriteCollection const& Sprites = renderables( ids.GetId( ActorVisual ) );
    Sprite const& Spr = Sprites( ids.GetId( Animation ) );
    SpritePhase const& Phase = Spr( state );

    TexturedBox( Dim, Phase, Col, Inserter );
}

}
