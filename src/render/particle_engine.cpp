#include "particle_engine.h"
#include "counter.h"
#include "shader_manager.h"
#include "vao_base.h"
#include "font.h"
#include "i_render.h"
#include "sprite.h"
#include "particle_template_repo.h"
#include "particle_layer_repo.h"
#include <boost/algorithm/clamp.hpp>

namespace render {
namespace {
struct Particle
{
    ParticleTemplate const* Template;
    glm::vec4 Color;
    glm::vec2 Pos;
    glm::vec2 Speed;
    glm::vec2 Acceleration;
    float Heading;
    float RotationSpeed;
    float RotationAcceleration;
    float Lifetime;
    float InitialLifetime;
    float Radius;
    Particle( ParticleTemplate const* pt, glm::vec2 const& pos, double ori );
};
Particle::Particle( ParticleTemplate const* ppt, glm::vec2 const& pos, double ori )
    : Template( ppt )
{
    ParticleTemplate const& pt = *ppt;
#define COLOR( channel ) \
    std::max<double>( 0.0, std::min<double>( 1.0, ( ( pt.Color.channel - pt.ColorVariance.channel / 2. ) + pt.ColorVariance.channel * ( RandomGenerator::global()() % 100 ) / 100. ) ) )
    Color = glm::vec4( COLOR( x ), COLOR( y ), COLOR( z ), COLOR( w ) );
#define INIT( member ) \
    member = ( pt.member - pt.member##Variance / 2. ) + ( pt.member##Variance * ( RandomGenerator::global()() % 100 ) / 100. )
    float AbsAcceleration;
    INIT( AbsAcceleration );
    INIT( RotationAcceleration );
#define ROLL_DIR( dir, opa, opb, val ) \
    if( pt.dir == ParticleTemplate::opa ) \
    { \
        val = - std::abs( val ); \
    } \
    else if( pt.dir == ParticleTemplate::opb ) \
    { \
        val = std::abs( val ); \
    } \
    else if( RandomGenerator::global()() % 2 == 1 ) \
    { \
        val = - val; \
    }
    ROLL_DIR( RotAccelerationDir, Rot_N, Rot_P, RotationAcceleration );
    INIT( RotationSpeed );
    ROLL_DIR( RotDir, Rot_N, Rot_P, RotationSpeed );
    float AbsSpeed;
    INIT( AbsSpeed );
    ROLL_DIR( SpeedDir, Towards, Away, AbsSpeed );
    ROLL_DIR( AccelerationDir, Towards, Away, AbsAcceleration );
    static const double pi = boost::math::constants::pi<double>();
    float dir = pi * 2 * ( RandomGenerator::global()() % 100 / 100. );
    Pos = pos + pt.PosVariance * ( ( RandomGenerator::global()() % 100 ) / 100.f ) * glm::vec2( cos( dir ), sin( dir ) );
    if( Pos != pos )
    {
        Speed = AbsSpeed * glm::normalize( Pos - pos );
        Acceleration = AbsAcceleration * glm::normalize( Pos - pos );
    }
    else
    {
        Speed = Acceleration = glm::vec2( 0, 0 );
    }
    Pos = glm::vec2( Pos.x, Pos.y );
    if( pt.Heading == ParticleTemplate::H_Actor )
    {
        Heading = ori;
    }
    else
    {
        Heading = pi * 2. * ( RandomGenerator::global()() % 101 ) / 100.;
    }
    INIT( Lifetime );
    InitialLifetime = Lifetime;
    INIT( Radius );
#undef INIT
#undef COLOR
#undef ROLL_DIR
}
typedef std::vector<Particle> Particles;
typedef std::vector<glm::vec2> Vec2s_t;
typedef std::vector<glm::vec4> Vec4s_t;
typedef std::vector<GLfloat> Floats_t;
bool getNextTextId( Particles::const_iterator& i, Particles::const_iterator e,
                    Vec2s_t& Positions, Vec4s_t& Colors, Vec4s_t& TexCoords,
                    Floats_t& Headings, Floats_t& Radii, Floats_t& Lifetimes,
                    render::RenderBatch& batch )
{
    // skip dead particles
    while( i != e && i->Lifetime < 0.0f )
    {
        ++i;
    }
    if( i == e )
    {
        return false;
    }
    Particle const& p = *i;
    Positions.push_back( p.Pos );
    SpritePhase const& Phase = p.Template->Spr->operator()( 100 * p.Lifetime / p.InitialLifetime );
    TexCoords.push_back( glm::vec4( Phase.Left, Phase.Right, Phase.Bottom, Phase.Top ) );
    Colors.push_back( p.Color );
    Headings.push_back( p.Heading );
    Radii.push_back( p.Radius );
    float lifetimeDivisor = p.InitialLifetime <= std::numeric_limits<float>::epsilon() ? 1.0f : p.InitialLifetime;
    Lifetimes.push_back( p.InitialLifetime >= 1.0f ? std::min( 1.0f, p.Lifetime ) : ( p.Lifetime / lifetimeDivisor ) );
    batch.TexId = Phase.TexId;
    ++i;
    return true;
}

}
struct ParticleEngineImpl
{
    typedef std::map<int32_t, Particles> ParticlesByLayer;
    ParticlesByLayer mParticlesByLayer;
    float mCycle;
    VaoBase mVAO;
    mutable size_t mPrevParticlesSize;
    ParticleEngineImpl();
    void Update( float dt );
    void Draw() const;
    void Draw( Particles const& particles ) const;
    void AddParticle( int32_t type, glm::vec2 const& pos, glm::vec2 const& distance, double ori );
};

ParticleEngineImpl::ParticleEngineImpl()
    : mCycle( 0.0f )
    , mPrevParticlesSize( 0 )
{
    mVAO.Init();
    ShaderManager& ShaderMgr( ShaderManager::Get() );
    static int32_t def( AutoId( "particle" ) );
    ShaderMgr.ActivateShader( def );
    ShaderMgr.UploadData( "spriteTexture", GLuint( 3 ) );
}

void ParticleEngineImpl::Update( float dt )
{
    mCycle += dt;
    for( ParticlesByLayer::iterator li = mParticlesByLayer.begin(), le = mParticlesByLayer.end(); li != le; ++li )
    {
        Particles& particles = li->second;
        for( Particles::iterator i = particles.begin(), e = particles.end(); i != e; ++i )
        {
            Particle& p = *i;
            p.Lifetime -= dt;
            if( p.Lifetime < 0 )
            {
                continue;
            }
            p.Pos += dt * p.Speed;
            p.Speed += dt * p.Acceleration;
            p.Heading += dt * p.RotationSpeed;
            p.RotationSpeed += dt * p.RotationAcceleration;
            p.Radius = boost::algorithm::clamp(
                p.Radius + dt * p.Template->ScaleSpeed,
                p.Template->MinRadius,
                p.Template->MaxRadius);
            
        }
        if( mCycle >= 10.f )
        {
            mCycle = 0.0f;
            particles.erase( std::remove_if( particles.begin(), particles.end(), [](Particle const& part){ return part.Lifetime < 0.; }), particles.end() );
        }
    }
}

/*
//---shader spec
layout(location=0) in vec4 TexCoord;
layout(location=1) in vec2 SpriteCenter;
layout(location=2) in float Heading;
layout(location=3) in vec4 Color;
layout(location=4) in float Radius;
layout(location=5) in float Lifetime;
*/
void ParticleEngineImpl::Draw() const
{
    glDepthMask( GL_FALSE );

    for( ParticlesByLayer::const_iterator li = mParticlesByLayer.begin(), le = mParticlesByLayer.end(); li != le; ++li )
    {
        static ParticleLayerRepo& plr = ParticleLayerRepo::Get();
        ParticleLayer const& pl = plr( li->first );
        glBlendFunc( pl.sfactor, pl.dfactor );
        Draw( li->second );
    }

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glDepthMask( GL_TRUE );
}

void ParticleEngineImpl::Draw( Particles const& particles ) const
{
    size_t CurSize = particles.size();
    if ( CurSize == 0 )
    {
        return;
    }

    Vec2s_t Positions;
    Vec4s_t Colors;
    Vec4s_t TexCoords;
    Floats_t Headings;
    Floats_t Radii;
    Floats_t Lifetimes;
    Positions.reserve( CurSize );
    Colors.reserve( CurSize );
    TexCoords.reserve( CurSize );
    Headings.reserve( CurSize );
    Radii.reserve( CurSize );
    Lifetimes.reserve( CurSize );

    Particles::const_iterator i = particles.begin();
    render::Counts_t const& Counts = render::count(
                                         std::bind( &getNextTextId, std::ref( i ), particles.end(),
                                                 std::ref( Positions ), std::ref( Colors ), std::ref( TexCoords ),
                                                 std::ref( Headings ), std::ref( Radii ), std::ref( Lifetimes ),
                                                 std::placeholders::_1 )
                                     );

    CurSize = Positions.size();
    if( CurSize == 0 )
    {
        return;
    }

    mVAO.Bind();

    if( CurSize != mPrevParticlesSize )
    {
        size_t TotalSize = CurSize * ( 2 * sizeof( glm::vec4 ) + sizeof( glm::vec2 ) + 3 * sizeof( GLfloat ) );
        glBufferData( GL_ARRAY_BUFFER, TotalSize, NULL, GL_DYNAMIC_DRAW );
        mPrevParticlesSize = CurSize;
    }

    size_t CurrentOffset = 0;
    size_t CurrentSize = 0;
    GLuint CurrentAttribIndex = 0;

    CurrentSize = CurSize * sizeof( glm::vec4 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &TexCoords[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    size_t const TexIndex = CurrentOffset;
    ++CurrentAttribIndex;
    CurrentOffset += CurrentSize;

    CurrentSize = CurSize * sizeof( glm::vec2 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Positions[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    size_t const PosIndex = CurrentOffset;
    ++CurrentAttribIndex;
    CurrentOffset += CurrentSize;

    CurrentSize = CurSize * sizeof( GLfloat );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Headings[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    size_t const HeadingIndex = CurrentOffset;
    ++CurrentAttribIndex;
    CurrentOffset += CurrentSize;

    CurrentSize = CurSize * sizeof( glm::vec4 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Colors[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    size_t const ColorIndex = CurrentOffset;
    ++CurrentAttribIndex;
    CurrentOffset += CurrentSize;

    CurrentSize = CurSize * sizeof( GLfloat );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Radii[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    size_t const RadiusIndex = CurrentOffset;
    ++CurrentAttribIndex;
    CurrentOffset += CurrentSize;

    CurrentSize = CurSize * sizeof( GLfloat );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Lifetimes[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    size_t const LifeIndex = CurrentOffset;
    ++CurrentAttribIndex;
    CurrentOffset += CurrentSize;

    ShaderManager& ShaderMgr( ShaderManager::Get() );
    static int32_t def( AutoId( "particle" ) );
    ShaderMgr.ActivateShader( def );
    glActiveTexture( GL_TEXTURE0 + 3 );
    for( render::Counts_t::const_iterator i = Counts.begin(), e = Counts.end(); i != e; ++i )
    {
        render::CountByTexId const& Part = *i;
        CurrentAttribIndex = 0;
        glVertexAttribPointer( CurrentAttribIndex, 4, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( TexIndex + sizeof( glm::vec4 )*Part.Start ) );
        glVertexAttribDivisor( CurrentAttribIndex, 1 );
        ++CurrentAttribIndex;
        glVertexAttribPointer( CurrentAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( PosIndex + sizeof( glm::vec2 )*Part.Start ) );
        glVertexAttribDivisor( CurrentAttribIndex, 1 );
        ++CurrentAttribIndex;
        glVertexAttribPointer( CurrentAttribIndex, 1, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( HeadingIndex + sizeof( GLfloat )*Part.Start ) );
        glVertexAttribDivisor( CurrentAttribIndex, 1 );
        ++CurrentAttribIndex;
        glVertexAttribPointer( CurrentAttribIndex, 4, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( ColorIndex + sizeof( glm::vec4 )*Part.Start ) );
        glVertexAttribDivisor( CurrentAttribIndex, 1 );
        ++CurrentAttribIndex;
        glVertexAttribPointer( CurrentAttribIndex, 1, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( RadiusIndex + sizeof( GLfloat )*Part.Start ) );
        glVertexAttribDivisor( CurrentAttribIndex, 1 );
        ++CurrentAttribIndex;
        glVertexAttribPointer( CurrentAttribIndex, 1, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( LifeIndex + sizeof( GLfloat )*Part.Start ) );
        glVertexAttribDivisor( CurrentAttribIndex, 1 );
        if( Part.Batch.TexId != -1 )
        {
            glBindTexture( GL_TEXTURE_2D, Part.Batch.TexId );
        }
        glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, 4, Part.Count );
    }
    mVAO.Unbind();
}

void ParticleEngineImpl::AddParticle( int32_t type, glm::vec2 const& pos, glm::vec2 const& distance, double ori )
{
    static ParticleTemplateRepo& ptr( ParticleTemplateRepo::Get() );
    ParticleTemplate const& pt = ptr( type );
    if( NULL == pt.Spr )
    {
        return;
    }
    Particles& particles = mParticlesByLayer[ pt.ParticleLayer ];
    for( int32_t i = 0, e = std::max<int>( 0, ( pt.Num - pt.NumVariance / 2 + ( pt.NumVariance * ( RandomGenerator::global()() % 100 ) ) / 100 ) ); i != e; ++i )
    {
        Particle p( &pt, pos, ori );
        if( p.Lifetime <= 0.0 )
        {
            continue;
        }
        if (pt.Interpolate)
        {
            p.Pos.x -= distance.x*i / e;
            p.Pos.y -= distance.y*i / e;
        }
        particles.push_back( p );
    }
}

ParticleEngine::ParticleEngine()
    : mImpl( new ParticleEngineImpl() )
{
}

ParticleEngine::~ParticleEngine()
{
}

void ParticleEngine::Update( float dt )
{
    mImpl->Update( dt );
}

void ParticleEngine::Draw() const
{
    mImpl->Draw();
}

void ParticleEngine::AddParticle( int32_t type, glm::vec2 const& pos, glm::vec2 const& distance, double ori )
{
    mImpl->AddParticle( type, pos, distance, ori );
}

} // namespace render

