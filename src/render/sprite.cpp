#include "i_render.h"
#include "sprite.h"
#include "texture_repo.h"

Sprite::Sprite( int32_t TexId, Json::Value const& Anim )
    : RepoBase( mDefaultSpritePhase )
    , mScale( 1.0 )
    , mSecsToEnd( 1 )
{
    Texture const& Tex = TextureRepo::Get()( TexId );
    mTexId = Tex.TexId();
    uint32_t w, h;
    Json::Value const& Phases = Anim["phases"];
    if( !Phases.isArray() )
    {
        mSteps = 0;
        return;
    }
    mSteps = Phases.size();
    if( Phases.empty() )
    {
        return;
    }
    if( !Json::GetUInt( Anim["w"], w ) ||
        !Json::GetUInt( Anim["h"], h ) )
    {
        assert( false );
        return;
    }
    GLfloat TexW = ( GLfloat )Tex.Width();
    GLfloat TexH = ( GLfloat )Tex.Height();
    if( TexW )
    {
        TexW = 1 / TexW;
    }
    if( TexH )
    {
        TexH = 1 / TexH;
    }
    int idx = 0;
    if( !Json::GetDouble( Anim["scale"], mScale ) )
    {
        mScale = 1.0;
    }
    if( !Json::GetDouble( Anim["secs_to_end"], mSecsToEnd ) )
    {
        mSecsToEnd = 1.0;
    }
    for( Json::Value::const_iterator i = Phases.begin(), e = Phases.end(); i != e; ++i, ++idx )
    {
        Json::Value const& Phase = *i;
        uint32_t sx, sy;
        if( !Json::GetUInt( Phase["x"], sx ) ||
            !Json::GetUInt( Phase["y"], sy ) )
        {
            assert( false );
            return;
        }
        mElements.insert( idx, new SpritePhase( mTexId, sy * TexH, sx * TexW, ( sy + h )*TexH, ( sx + w )*TexW ) );
    }
}

SpritePhase const& Sprite::operator()( int32_t Phase ) const
{
    int32_t SprPhase = ( int32_t )(( Phase * mSteps ) / 100. );
    return RepoBase::operator()( SprPhase );
}

bool Sprite::IsValid() const
{
    return mTexId != 0;
}

double Sprite::GetScale() const
{
    return mScale;
}

double Sprite::GetSecsToEnd() const
{
    return mSecsToEnd;
}

