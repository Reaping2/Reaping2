#include "i_render.h"
#include "uimodel.h"

void UiModel::CollectVertices( Widget const& Wdg, UiVertexInserter_t& Inserter )const
{
    glm::vec4 const& Dim = Wdg.GetDimensions();
    glm::vec4 const& Col = GetColor( Wdg );
    ColoredBox( Dim, Col, Inserter );
}

void UiModel::ColoredBox( glm::vec4 const& Dim, glm::vec4 const& Col, UiVertexInserter_t& Inserter )
{
    *Inserter++ = UiVertex( glm::vec2( Dim.x, Dim.y ), Col );
    *Inserter++ = UiVertex( glm::vec2( Dim.x, Dim.y + Dim.w ), Col );
    *Inserter++ = UiVertex( glm::vec2( Dim.x + Dim.z, Dim.y ), Col );
    *Inserter++ = UiVertex( glm::vec2( Dim.x + Dim.z, Dim.y ), Col );
    *Inserter++ = UiVertex( glm::vec2( Dim.x, Dim.y + Dim.w ), Col );
    *Inserter++ = UiVertex( glm::vec2( Dim.x + Dim.z, Dim.y + Dim.w ), Col );
}

glm::vec4 UiModel::GetColor( Widget const& Wdg )
{
    uint32_t const Color = ( int32_t )( Wdg( Widget::PT_Highlight ) ) ? ( int32_t )( Wdg( Widget::PT_HighlightColor ) ) : ( int32_t )( Wdg( Widget::PT_Color ) );
    return glm::vec4( ( ( Color >> 24 ) & 0xff ) / 255.,
                      ( ( Color >> 16 ) & 0xff ) / 255.,
                      ( ( Color >> 8 ) & 0xff ) / 255.,
                      ( Color & 0xff ) / 255. );
}

void UiModel::TexturedBox( glm::vec4 const& Dim, SpritePhase const& Phase, glm::vec4 const& Col, GLuint TexId, UiVertexInserter_t& Inserter )
{
    *Inserter++ = UiVertex( glm::vec2( Dim.x, Dim.y ), glm::vec2( Phase.Left, Phase.Bottom ), Col, TexId );
    *Inserter++ = UiVertex( glm::vec2( Dim.x, Dim.y + Dim.w ), glm::vec2( Phase.Left, Phase.Top ), Col, TexId );
    *Inserter++ = UiVertex( glm::vec2( Dim.x + Dim.z, Dim.y ), glm::vec2( Phase.Right, Phase.Bottom ), Col, TexId );
    *Inserter++ = UiVertex( glm::vec2( Dim.x + Dim.z, Dim.y ), glm::vec2( Phase.Right, Phase.Bottom ), Col, TexId );
    *Inserter++ = UiVertex( glm::vec2( Dim.x, Dim.y + Dim.w ), glm::vec2( Phase.Left, Phase.Top ), Col, TexId );
    *Inserter++ = UiVertex( glm::vec2( Dim.x + Dim.z, Dim.y + Dim.w ), glm::vec2( Phase.Right, Phase.Top ), Col, TexId );
}

