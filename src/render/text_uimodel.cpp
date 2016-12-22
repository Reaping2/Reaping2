#include "i_render.h"
#include "text_uimodel.h"
#include "main/window.h"
#include "engine/engine.h"
#include "text.h"
#include "font.h"


namespace {
void scale( float& x, float& y, float ratio )
{
    if( ratio >= 1.0f )
    {
        x /= ratio;
    }
    else
    {
        y *= ratio;
    }
}
}

bool TextUiModel::CalcRequiredSize( Widget const& Wdg, glm::vec2& OutScaler, glm::vec2& OutReqSize, std::string& OutBuf, float Ratio )
{
    Text text( Wdg( Widget::PT_FontSize ), Wdg.GetDimensions(), GetColor( Wdg ), Wdg( Widget::PT_Text ), false );
    return CalcRequiredSize( text, OutScaler, OutReqSize, OutBuf, Ratio );
}

bool TextUiModel::CalcRequiredSize( Text const& text, glm::vec2& OutScaler, glm::vec2& OutReqSize, std::string& OutBuf, float Ratio )
{
    static Font& Fnt( Font::Get() );
    double FontSize = text.mFontSize;
    double TexFontSize = Fnt.GetFontSize();
    glm::vec4 const& Dim = text.mDimensions;
    if( TexFontSize <= std::numeric_limits<float>::epsilon() )
    {
        return false;
    }
    glm::vec2 RequiredSize;
    glm::vec2 TexDim;
    std::string Buf = text.mText;
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
        scale( RequiredSize.x, RequiredSize.y, Ratio );
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
    OutReqSize = RequiredSize;
    RequiredSize.y = RequiredSize.x;
    RequiredSize.x /= TexDim.x;
    OutScaler = RequiredSize;
    OutBuf = Buf;
    return true;
}

void TextUiModel::CollectVertices( Widget const& Wdg, UiVertexInserter_t& Inserter )const
{
    Text text( Wdg( Widget::PT_FontSize ), Wdg.GetDimensions(), GetColor( Wdg ), Wdg( Widget::PT_Text ), false );
    text.mVAlign = (TextWidget::VAlign)( (int)Wdg( Widget::PT_VAlign ) );
    text.mHAlign = (TextWidget::HAlign)( (int)Wdg( Widget::PT_HAlign ) );
    CollectVertices( text, Inserter, true );
}

void TextUiModel::CollectVertices( Text const& text, UiVertexInserter_t& Inserter, bool ScaleWithRatio )
{
    int w, h;
    engine::Engine::Get().GetSystem<engine::WindowSystem>()->GetWindowSize( w, h );
    float Ratio = ( ScaleWithRatio && h != 0 ) ? ( 1.0f * w / h ) : 1.0f;
    glm::vec2 RequiredSize, Scaler;
    std::string Buf;
    if( !CalcRequiredSize( text, Scaler, RequiredSize, Buf, Ratio ) )
    {
        return;
    }
    glm::vec4 const& Col = text.mColor;
    glm::vec4 Dim = text.mDimensions;
    switch( text.mHAlign )
    {
        case TextWidget::HAlign::Center:
            Dim.x += ( Dim.z - RequiredSize.x ) / 2.0;
            break;
        case TextWidget::HAlign::Right:
            Dim.x += ( Dim.z - RequiredSize.x );
            break;
        default:
            break;
    }

    switch( text.mVAlign )
    {
        case TextWidget::VAlign::Mid:
            Dim.y += ( Dim.w - RequiredSize.y ) / 2.0;
            break;
        case TextWidget::VAlign::Bottom:
            Dim.y += ( Dim.w - RequiredSize.y );
            break;
        default:
            break;
    }

    static Font& Fnt( Font::Get() );
    for( std::string::const_iterator i = Buf.begin(), e = Buf.end(); i != e; ++i )
    {
        Character const& Char = Fnt.GetChar( *i );
        SpritePhase const& Phase = Char.Phase;
        Dim.z = Scaler.x * Char.Size.x;
        Dim.w = Scaler.x * Ratio * Char.Size.y;
        TexturedBox( Dim, Phase, Col, Inserter );
        Dim.x += Dim.z;
    }
}

