#include "i_render.h"
#include "font.h"
#include "texture.h"
#include "texture_repo.h"
#include "sprite_phase.h"

Font::Font()
    : mFirstChar( 0 )
    , mLastChar( 0 )
    , mTexId( 0 )
    , mMaxHeight( 0 )
{
    Load( "misc/font.json" );
}

void Font::Load( std::string const& Path )
{
    AutoFile Fnt = Filesys::Get().Open( Path );
    if( !Fnt.get() || !Fnt->IsValid() )
    {
        return;
    }
    JsonReader Reader( *Fnt );
    if( !Reader.IsValid() )
    {
        return;
    }
    Json::Value& Root = Reader.GetRoot();
    if( !Root.isObject() )
    {
        return;
    }
    std::string TexName;
    if( !GetStr( Root["file"], TexName ) )
    {
        return;
    }
    Texture const& Tex = TextureRepo::Get()( AutoId( boost::filesystem::path( TexName ).generic_string() ) );
    if( !Tex.TexId() || !Tex.Width() || !Tex.Height() )
    {
        return;
    }
    Json::Value& Characters = Root["characters"];
    if( !Characters.isArray() || Characters.empty() )
    {
        return;
    }
    const int Size = ( int )Characters.size();
    CharDesc FirstChar, LastChar;
    if( !LoadChar( FirstChar, Tex, Characters[0] ) || !LoadChar( LastChar, Tex, Characters[Size - 1] ) )
    {
        return;
    }
    Characters_t Chars( LastChar.Code - FirstChar.Code + 1 );
    Chars[0] = FirstChar.Phase;
    Chars[Size - 1] = LastChar.Phase;
    mMaxHeight = 0;
    for( int i = 1; i < Size - 1; ++i )
    {
        CharDesc Desc;
        if( !LoadChar( Desc, Tex, Characters[i] ) )
        {
            return;
        }
        assert( Desc.Code >= FirstChar.Code && Desc.Code <= LastChar.Code );
        GLfloat const Height = Desc.Phase.Size.y;
        if( Height > mMaxHeight )
        {
            mMaxHeight = Height;
        }
        Chars[Desc.Code - FirstChar.Code] = Desc.Phase;
    }
    if( mMaxHeight <= std::numeric_limits<float>::epsilon() )
    {
        return;
    }
    mFirstChar = FirstChar.Code;
    mLastChar = LastChar.Code;
    using std::swap;
    swap( mChars, Chars );
    mTexId = Tex.TexId();
}

bool Font::LoadChar( CharDesc& Character, Texture const& Tex, Json::Value& CharValue )
{
    if( !CharValue.isObject() )
    {
        return false;
    }
    const GLfloat TW = 1.f / Tex.Width();
    const GLfloat TH = 1.f / Tex.Height();
    uint32_t UiVal;
    if( !GetUInt( CharValue["code"], UiVal ) )
    {
        return false;
    }
    uint32_t X, Y, W, H;
    if( !Json::GetUInt( CharValue["x"], X ) ||
        !Json::GetUInt( CharValue["y"], Y ) ||
        !Json::GetUInt( CharValue["width"], W ) ||
        !Json::GetUInt( CharValue["height"], H ) )
    {
        return false;
    }

    Character.Code = UiVal;
    Character.Phase.Phase = SpritePhase( Tex.TexId(), Y * TH, X * TW, ( Y + H ) * TH, ( X + W ) * TW );
    Character.Phase.Size = glm::vec2( W, H );
    return true;
}

Character const& Font::GetChar( char C )const
{
    if( C < mFirstChar || C > mLastChar )
    {
        return mDefaultChar;
    }
    return mChars[C - mFirstChar];
}

glm::vec2 Font::GetDim( std::string const& Text ) const
{
    glm::vec2 Dim;
    for( std::string::const_iterator i = Text.begin(), e = Text.end(); i != e; ++i )
    {
        Character const& Spr = GetChar( *i );
        Dim.x += Spr.Size.x;
        Dim.y = std::max<float>( Dim.y, Spr.Size.y );
    }
    return Dim;
}

GLfloat Font::GetFontSize() const
{
    return mMaxHeight;
}

