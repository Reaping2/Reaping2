#include "i_render.h"
#include "bar_model.h"

void BarModel::CollectVertices( const Widget& Wdg, UiVertexInserter_t& Inserter ) const
{
    int32_t MaxProg = Wdg( Widget::PT_MaxProgress );
    int32_t Prog = Wdg( Widget::PT_Progress );
    float Percent = std::min<float>( 1.0f, std::max<float>( 0.0f, MaxProg ? ( ( float )Prog / MaxProg ) : 1.0f ) );
    glm::vec4 Dim = Wdg.GetDimensions();
    Dim.z *= Percent;
    glm::vec4 const& Col = GetColor( Wdg );
    ColoredBox( Dim, Col, Inserter );
}

