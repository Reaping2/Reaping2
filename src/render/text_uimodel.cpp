#include "i_render.h"

bool TextUiModel::CalcRequiredSize( Widget const& Wdg, glm::vec2& OutReqSize, std::string& OutBuf )
{
    static Font& Fnt( Font::Get() );
    double FontSize = Wdg( Widget::PT_FontSize );
    double TexFontSize = Fnt.GetFontSize();
    glm::vec4 const& Dim = Wdg.GetDimensions();
    if( TexFontSize <= std::numeric_limits<float>::epsilon() )
    {
        return false;
    }
    glm::vec2 RequiredSize;
    glm::vec2 TexDim;
    std::string Buf = Wdg( Widget::PT_Text );
    do
    {
        if( Buf.empty() )
        {
            return false;
        }
        TexDim = Fnt.GetDim( Buf );
        if( TexDim.x <= std::numeric_limits<float>::epsilon() )
        {
            return false;
        }
        RequiredSize = TexDim;
        RequiredSize *= ( FontSize / TexFontSize );
        if( RequiredSize.x <= Dim.z && RequiredSize.y <= Dim.w )
        {
            break;
        }
        if( Buf.size() < 4 )
        {
            return false;
        }
        Buf = Buf.substr( 0, Buf.size() - 4 );
        Buf += "...";
    }
    while( true );
    RequiredSize.x /= TexDim.x;
    OutReqSize = RequiredSize;
    OutBuf = Buf;
    return true;
}

void TextUiModel::CollectVertices( Widget const& Wdg, UiVertexInserter_t& Inserter )const
{
    glm::vec2 RequiredSize;
    std::string Buf;
    if( !CalcRequiredSize( Wdg, RequiredSize, Buf ) )
    {
        return;
    }
    glm::vec4 const& Col = GetColor( Wdg );
    glm::vec4 Dim = Wdg.GetDimensions();
    static Font& Fnt( Font::Get() );
    GLuint const TexId = Fnt.GetTexId();
    for( std::string::const_iterator i = Buf.begin(), e = Buf.end(); i != e; ++i )
    {
        SpritePhase const& Phase = Fnt.GetChar( *i );
        Dim.z = RequiredSize.x * ( Phase.Right - Phase.Left );
        Dim.w = RequiredSize.x * ( Phase.Bottom - Phase.Top );
        TexturedBox( Dim, Phase, Col, TexId, Inserter );
        Dim.x += Dim.z;
    }
}

