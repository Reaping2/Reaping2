#include "i_render.h"
#include "decal_engine.h"
#include "shader_manager.h"
#include "sprite.h"
#include "sprite_phase.h"

DecalEngine::DecalEngine()
    : mRenderables( RenderableRepo::Get() )
{
    mMaxDecalsPerType = 10000;
    for( size_t i = 0; i < NumTypes; ++i )
    {
        mNextIdx[i] = 0;
        mNumDecals[i] = 0;
    }
    mVAO.Init();
    mVAO.Bind();
    mOneTypeSize = mMaxDecalsPerType * ( sizeof( glm::vec2 ) + sizeof( glm::vec4 ) );
    glBufferData( GL_ARRAY_BUFFER, NumTypes * mOneTypeSize, NULL, GL_DYNAMIC_DRAW );
    mVAO.Unbind();
}

void DecalEngine::Add( Decal const& Part, DecalType Typ )
{
    mNewDecals[Typ].push_back( Part );
}

void DecalEngine::UpdateBuffers( DecalType Typ )
{
    // todo create update method, put data upload there
    Particles_t& Parts = mNewDecals[Typ];
    if( Parts.empty() )
    {
        return;
    }

    mVAO.Bind();
    std::vector<glm::vec2> PosBuf;
    std::vector<glm::vec4> TexBuf;
    PosBuf.reserve( Parts.size() );
    TexBuf.reserve( Parts.size() );
    for( Particles_t::const_iterator b = Parts.begin(), i = b, e = Parts.end(); i != e; ++i )
    {
        Decal const& p = *i;
        static int32_t DefaultActId = AutoId( "default_action" );
        Sprite const& Spr = mRenderables( p.mId )( DefaultActId );
        if( !Spr.IsValid() )
        {
            continue;
        }
        SpritePhase const& Phase = Spr( 0 );
        mTexId = Phase.TexId;
        PosBuf.push_back( p.mCenter );
        TexBuf.push_back( glm::vec4( Phase.Left, Phase.Right, Phase.Bottom, Phase.Top ) );
    }
    size_t Remaining = TexBuf.size();
    size_t& WhereToAppend = mNextIdx[Typ];
    size_t NumToAppend = std::min<size_t>( Remaining, mMaxDecalsPerType - WhereToAppend );
    while( NumToAppend )
    {
        glBufferSubData( GL_ARRAY_BUFFER, Typ * mOneTypeSize + WhereToAppend * sizeof( glm::vec4 ), NumToAppend * sizeof( glm::vec4 ), &TexBuf[0] );
        glBufferSubData( GL_ARRAY_BUFFER, Typ * mOneTypeSize + mMaxDecalsPerType * sizeof( glm::vec4 ) + WhereToAppend * sizeof( glm::vec2 ), NumToAppend * sizeof( glm::vec2 ), &PosBuf[0] );
        WhereToAppend += NumToAppend;
        if( WhereToAppend >= mMaxDecalsPerType )
        {
            WhereToAppend = 0;
        }
        size_t NextRound = std::min<size_t>( Remaining - NumToAppend, mMaxDecalsPerType - WhereToAppend );
        Remaining -= NumToAppend;
        NumToAppend = NextRound;
    }
    mNumDecals[Typ] += TexBuf.size();
    if( mNumDecals[Typ] > mMaxDecalsPerType )
    {
        mNumDecals[Typ] = mMaxDecalsPerType;
    }
    mVAO.Unbind();
    Parts.clear();
}

void DecalEngine::Draw( DecalType Type )
{
    UpdateBuffers( Type );
    if( !mNumDecals[Type] )
    {
        return;
    }
    mVAO.Bind();
    ShaderManager& ShaderMgr( ShaderManager::Get() );
    ShaderMgr.ActivateShader( "decal" );
    glActiveTexture( GL_TEXTURE0 + 3 );
    //todo: bind only once
    glBindTexture( GL_TEXTURE_2D, mTexId );
    ShaderMgr.UploadData( "spriteTexture", GLuint( 3 ) );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( Type * mOneTypeSize ) );
    glVertexAttribDivisor( 0, 1 );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( Type * mOneTypeSize + mMaxDecalsPerType * sizeof( glm::vec4 ) ) );
    glVertexAttribDivisor( 1, 1 );
    glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, 4, mNumDecals[Type] );
    glActiveTexture( GL_TEXTURE0 );
    mVAO.Unbind();
}

