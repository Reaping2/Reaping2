#include "i_render.h"
#include "decal_engine.h"
#include "shader_manager.h"
#include "sprite.h"
#include "sprite_phase.h"

DecalEngine::DecalEngine()
    : mRenderables( RenderableRepo::Get() )
    , mTexIndex( 0 )
    , mPosIndex( 0 )
    , mHeadingIndex( 0 )
    , mAlphaIndex( 0 )
    , mRadiusIndex( 0 )
    , mPrevDecalsSize( 0 )
    , mDirty( false )
{
    mMaxDecalsPerType = 10000;
    mVAO.Init();
    ShaderManager& ShaderMgr( ShaderManager::Get() );
    static int32_t def( AutoId( "decals" ) );
    ShaderMgr.ActivateShader( def );
    ShaderMgr.UploadData( "spriteTexture", GLuint( 3 ) );
}

void DecalEngine::Add( Decal const& Part, DecalType Typ )
{
    mDirty = true;
    mDecals.push_back( Part );
    // TODO
    // check max num of possible decals
    // slowly fade out the first decals with decresing alpha
}

namespace {
typedef std::vector<Decal> Decals_t;
typedef std::vector<glm::vec2> Vec2s_t;
typedef std::vector<glm::vec4> Vec4s_t;
typedef std::vector<GLfloat> Floats_t;
bool getNextTextId( Decals_t::const_iterator& i, Decals_t::const_iterator e,
                    Vec2s_t& Positions, Floats_t& Alphas, Vec4s_t& TexCoords,
                    Floats_t& Headings, Floats_t& Radii,
                    render::RenderBatch& batch )
{
    if( i == e )
    {
        return false;
    }
    auto const& p = *i;
    Positions.push_back( p.mCenter );
    TexCoords.push_back( p.mTexCoords );
    Headings.push_back( p.mHeading );
    Radii.push_back( p.mRadius );
    Alphas.push_back( p.mAlpha );
    batch.TexId = p.mTexId;
    ++i;
    return true;
}
}
void DecalEngine::UpdateBuffers()
{
    if( !mDirty || mDecals.empty() )
    {
        return;
    }
    mDirty = false;
    size_t CurSize = mDecals.size();

    Vec2s_t Positions;
    Vec4s_t TexCoords;
    Floats_t Headings;
    Floats_t Radii;
    Floats_t Alphas;
    Positions.reserve( CurSize );
    Alphas.reserve( CurSize );
    TexCoords.reserve( CurSize );
    Headings.reserve( CurSize );
    Radii.reserve( CurSize );

    Decals_t::const_iterator i = mDecals.begin();
    mCounts = render::count(
                  std::bind( &getNextTextId, std::ref( i ), mDecals.end(),
                           std::ref( Positions ), std::ref( Alphas ), std::ref( TexCoords ),
                           std::ref( Headings ), std::ref( Radii ),
                           std::placeholders::_1 )
              );


    mVAO.Bind();

    if( CurSize != mPrevDecalsSize )
    {
        size_t TotalSize = CurSize * ( sizeof( glm::vec4 ) + sizeof( glm::vec2 ) + 3 * sizeof( GLfloat ) );
        glBufferData( GL_ARRAY_BUFFER, TotalSize, NULL, GL_DYNAMIC_DRAW );
        mPrevDecalsSize = CurSize;
    }

    size_t CurrentOffset = 0;
    size_t CurrentSize = 0;
    GLuint CurrentAttribIndex = 0;

    CurrentSize = CurSize * sizeof( glm::vec4 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &TexCoords[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    mTexIndex = CurrentOffset;
    ++CurrentAttribIndex;
    CurrentOffset += CurrentSize;

    CurrentSize = CurSize * sizeof( glm::vec2 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Positions[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    mPosIndex = CurrentOffset;
    ++CurrentAttribIndex;
    CurrentOffset += CurrentSize;

    CurrentSize = CurSize * sizeof( GLfloat );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Headings[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    mHeadingIndex = CurrentOffset;
    ++CurrentAttribIndex;
    CurrentOffset += CurrentSize;

    CurrentSize = CurSize * sizeof( GLfloat );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Alphas[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    mAlphaIndex = CurrentOffset;
    ++CurrentAttribIndex;
    CurrentOffset += CurrentSize;

    CurrentSize = CurSize * sizeof( GLfloat );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Radii[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    mRadiusIndex = CurrentOffset;
    ++CurrentAttribIndex;
    CurrentOffset += CurrentSize;

    mVAO.Unbind();
}

void DecalEngine::Draw()
{
    UpdateBuffers();
    if( mDecals.empty() )
    {
        return;
    }
    mVAO.Bind();
    ShaderManager& ShaderMgr( ShaderManager::Get() );
    static int32_t def( AutoId( "decals" ) );
    ShaderMgr.ActivateShader( def );
    glActiveTexture( GL_TEXTURE0 + 3 );
    for( render::Counts_t::const_iterator i = mCounts.begin(), e = mCounts.end(); i != e; ++i )
    {
        render::CountByTexId const& Part = *i;
        GLuint CurrentAttribIndex = 0;
        glVertexAttribPointer( CurrentAttribIndex, 4, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mTexIndex + sizeof( glm::vec4 )*Part.Start ) );
        glVertexAttribDivisor( CurrentAttribIndex, 1 );
        ++CurrentAttribIndex;
        glVertexAttribPointer( CurrentAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mPosIndex + sizeof( glm::vec2 )*Part.Start ) );
        glVertexAttribDivisor( CurrentAttribIndex, 1 );
        ++CurrentAttribIndex;
        glVertexAttribPointer( CurrentAttribIndex, 1, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mHeadingIndex + sizeof( GLfloat )*Part.Start ) );
        glVertexAttribDivisor( CurrentAttribIndex, 1 );
        ++CurrentAttribIndex;
        glVertexAttribPointer( CurrentAttribIndex, 1, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mAlphaIndex + sizeof( GLfloat )*Part.Start ) );
        glVertexAttribDivisor( CurrentAttribIndex, 1 );
        ++CurrentAttribIndex;
        glVertexAttribPointer( CurrentAttribIndex, 1, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mRadiusIndex + sizeof( GLfloat )*Part.Start ) );
        glVertexAttribDivisor( CurrentAttribIndex, 1 );
        if( Part.Batch.TexId != -1 )
        {
            glBindTexture( GL_TEXTURE_2D, Part.Batch.TexId );
        }
        glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, 4, Part.Count );
    }

    glActiveTexture( GL_TEXTURE0 );
    mVAO.Unbind();
}

