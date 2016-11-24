#include "sprite_phase_cache.h"
#include "render_target.h"
#include "shader_manager.h"
#include "platform/log.h"
#include "platform/settings.h"

namespace render {

SpritePhaseCache::SpritePhaseCache()
{
    static Settings& settings( Settings::Get() );
    mMaxCellSize = settings.GetInt( "graphics.sprite_phase_cache.max_cell_size", 128 );
    static RenderTarget& rt( RenderTarget::Get() );
    mTarget = rt.GetFreeId();
    uint32_t current = rt.GetCurrentTarget();
    mTargetSize = rt.GetMaxTextureSize();
    mRowSize = std::floor( mTargetSize.x / mMaxCellSize );
    L1( "Texture size: %d x %d\n", (int)mTargetSize.x, (int)mTargetSize.y );
    rt.SetTargetTexture( mTarget, mTargetSize );
    rt.SelectTargetTexture( current );
    mTargetTexId = rt.GetTextureId( mTarget );
    mVAO.Init();
    mVAO.Bind();
    size_t TotalSize = 4 * sizeof( glm::vec2 );
    glBufferData( GL_ARRAY_BUFFER, 2 * TotalSize, NULL, GL_DYNAMIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, 0 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, ( void* )( TotalSize ) );
    mVAO.Unbind();
}

glm::vec4 SpritePhaseCache::FindFreeRegion( SpritePhase const& sprphase )
{
    if( mCacheIndex >= mRowSize * mRowSize )
    {
        return glm::vec4();
    }


    glm::vec4 rv(
            mCacheIndex / mRowSize * mMaxCellSize / mTargetSize.x,
            mCacheIndex % mRowSize * mMaxCellSize / mTargetSize.y,
            ( mCacheIndex / mRowSize + 1 ) * mMaxCellSize / mTargetSize.x,
            ( mCacheIndex % mRowSize + 1 ) * mMaxCellSize / mTargetSize.y
        );
    ++mCacheIndex;
    return rv;
}

namespace {
void DoDraw( std::vector<glm::vec2> const& Positions,
        std::vector<glm::vec2> const& TexCoords )
{
    size_t CurrentOffset = 0;
    size_t CurrentSize = 0;
    GLuint CurrentAttribIndex = 0;
    size_t CurSize = Positions.size();

    CurrentSize = CurSize * sizeof( glm::vec2 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &TexCoords[0] );
    size_t const TexIndex = CurrentOffset;
    CurrentOffset += CurrentSize;

    CurrentSize = CurSize * sizeof( glm::vec2 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Positions[0] );
    size_t const PosIndex = CurrentOffset;

    glDrawArrays( GL_TRIANGLE_STRIP, 0, CurSize );
}

}

void SpritePhaseCache::Draw( SpritePhase const& sprphase, glm::vec4 const& freeRegion, glm::vec4 const& scaledRegion )
{
    glBindTexture( GL_TEXTURE_2D, sprphase.TexId );

    std::vector<glm::vec2> Positions;
    std::vector<glm::vec2> TexCoords;

    L1( "region: %f %f %f %f\n",
            freeRegion.x,
            freeRegion.y,
            freeRegion.z,
            freeRegion.w );

// mid
    Positions.emplace_back( scaledRegion.x, scaledRegion.y );
    Positions.emplace_back( scaledRegion.x, scaledRegion.w );
    Positions.emplace_back( scaledRegion.z, scaledRegion.y );
    Positions.emplace_back( scaledRegion.z, scaledRegion.w );

    TexCoords.emplace_back( sprphase.Left, sprphase.Bottom );
    TexCoords.emplace_back( sprphase.Left, sprphase.Top );
    TexCoords.emplace_back( sprphase.Right, sprphase.Bottom );
    TexCoords.emplace_back( sprphase.Right, sprphase.Top );

    DoDraw( Positions, TexCoords );
    static Settings& settings( Settings::Get() );
    static bool gapless = settings.GetInt( "graphics.sprite_phase_cache.gapless", 1 ) != 0;
    if( !gapless )
    {
        return;
    }
// left
    Positions.clear();
    TexCoords.clear();

    Positions.emplace_back( freeRegion.x, scaledRegion.y );
    Positions.emplace_back( freeRegion.x, scaledRegion.w );
    Positions.emplace_back( scaledRegion.x, scaledRegion.y );
    Positions.emplace_back( scaledRegion.x, scaledRegion.w );

    TexCoords.emplace_back( sprphase.Left, sprphase.Bottom );
    TexCoords.emplace_back( sprphase.Left, sprphase.Top );
    TexCoords.emplace_back( sprphase.Left, sprphase.Bottom );
    TexCoords.emplace_back( sprphase.Left, sprphase.Top );

    DoDraw( Positions, TexCoords );
// right
    Positions.clear();
    TexCoords.clear();

    Positions.emplace_back( scaledRegion.z, scaledRegion.y );
    Positions.emplace_back( scaledRegion.z, scaledRegion.w );
    Positions.emplace_back( freeRegion.z, scaledRegion.y );
    Positions.emplace_back( freeRegion.z, scaledRegion.w );

    TexCoords.emplace_back( sprphase.Right, sprphase.Bottom );
    TexCoords.emplace_back( sprphase.Right, sprphase.Top );
    TexCoords.emplace_back( sprphase.Right, sprphase.Bottom );
    TexCoords.emplace_back( sprphase.Right, sprphase.Top );

    DoDraw( Positions, TexCoords );
// top
    Positions.clear();
    TexCoords.clear();

    Positions.emplace_back( scaledRegion.x, scaledRegion.w );
    Positions.emplace_back( scaledRegion.x, freeRegion.w );
    Positions.emplace_back( scaledRegion.z, scaledRegion.w );
    Positions.emplace_back( scaledRegion.z, freeRegion.w );

    TexCoords.emplace_back( sprphase.Left, sprphase.Top );
    TexCoords.emplace_back( sprphase.Left, sprphase.Top );
    TexCoords.emplace_back( sprphase.Right, sprphase.Top );
    TexCoords.emplace_back( sprphase.Right, sprphase.Top );

    DoDraw( Positions, TexCoords );
// bottom
    Positions.clear();
    TexCoords.clear();

    Positions.emplace_back( scaledRegion.x, freeRegion.y );
    Positions.emplace_back( scaledRegion.x, scaledRegion.y );
    Positions.emplace_back( scaledRegion.z, freeRegion.y );
    Positions.emplace_back( scaledRegion.z, scaledRegion.y );

    TexCoords.emplace_back( sprphase.Left, sprphase.Bottom );
    TexCoords.emplace_back( sprphase.Left, sprphase.Bottom );
    TexCoords.emplace_back( sprphase.Right, sprphase.Bottom );
    TexCoords.emplace_back( sprphase.Right, sprphase.Bottom );

    DoDraw( Positions, TexCoords );
}

namespace {
glm::vec4 ScaleRegion( glm::vec4 freeRegion, float scale )
{
    auto dx = freeRegion.z - freeRegion.x;
    freeRegion.x += dx * ( 1 - scale ) / 2;
    freeRegion.z = freeRegion.x + scale * dx;
    auto dy = freeRegion.w - freeRegion.y;
    freeRegion.y += dy * ( 1 - scale ) / 2;
    freeRegion.w = freeRegion.y + scale * dy;
    return freeRegion;
}
}

void SpritePhaseCache::Request( SpritePhase const& sprphase, float size )
{
    static Settings& settings( Settings::Get() );
    static bool enabled = settings.GetInt( "graphics.sprite_phase_cache.enabled", 1 ) != 0;
    if( !enabled )
    {
        return;
    }
    if( sprphase.TexId == mTargetTexId )
    {
        return;
    }
    static bool useSizeLimit = settings.GetInt( "graphics.sprite_phase_cache.use_size_limit", 1 ) != 0;
    if( useSizeLimit && size > mMaxCellSize )
    {
        return;
    }
    static float downsample = settings.GetFloat( "graphics.sprite_phase_cache.downsample", 1 );
    size *= downsample;
    glm::vec4 freeRegion = FindFreeRegion( sprphase );
    if( freeRegion.x == 0 &&
        freeRegion.z == 0 &&
        freeRegion.y == 0 &&
        freeRegion.w == 0 )
    {
        return;
    }
    auto const& scaledRegion = ScaleRegion( freeRegion, size / mMaxCellSize );
    // todo: max?
    mPending[ &sprphase ] = ExtData{ freeRegion, scaledRegion, size };
}

void SpritePhaseCache::ProcessPending()
{
    if( mPending.empty() )
    {
        return;
    }
    static RenderTarget& rt( RenderTarget::Get() );
    rt.SelectTargetTexture( mTarget );
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glViewport( 0, 0, mTargetSize.x, mTargetSize.y );
    ShaderManager& ShaderMgr( ShaderManager::Get() );
    ShaderMgr.ActivateShader( "combiner" );
    ShaderMgr.UploadData( "texture", GLuint( 1 ) );
    glActiveTexture( GL_TEXTURE0 + 1 );
    mVAO.Bind();

    for( auto const& vt : mPending )
    {
        auto const& sprphase = *vt.first;
        mOriginal[ &sprphase ] = sprphase;

        // copy the sprite to the temp map
        auto const& freeRegion = vt.second.freeRegion;
        auto const& scaledRegion = vt.second.scaledRegion;

        Draw( sprphase, freeRegion, scaledRegion );

        SpritePhase& mutablePhase = const_cast<SpritePhase&>( sprphase );

        L1( "spr: %d %f %f %f %f\n",
                mutablePhase.TexId,
                mutablePhase.Left,
                mutablePhase.Bottom,
                mutablePhase.Right,
                mutablePhase.Top );

        mutablePhase.Left = scaledRegion.x;
        mutablePhase.Bottom = scaledRegion.y;
        mutablePhase.Right = scaledRegion.z;
        mutablePhase.Top = scaledRegion.w;

        mutablePhase.TexId = mTargetTexId;
        L1( "spr post: %d %f %f %f %f\n",
                mutablePhase.TexId,
                mutablePhase.Left,
                mutablePhase.Bottom,
                mutablePhase.Right,
                mutablePhase.Top );

    }
    mVAO.Unbind();
    glActiveTexture( GL_TEXTURE0 );
    mPending.clear();
}
}


