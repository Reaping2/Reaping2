#include "i_render.h"

namespace {
void printShaderInfoLog( GLuint obj )
{
    int infologLength = 0;
    int charsWritten  = 0;
    char* infoLog;

    glGetShaderiv( obj, GL_INFO_LOG_LENGTH, &infologLength );

    if ( infologLength > 0 )
    {
        infoLog = ( char* )malloc( infologLength );
        glGetShaderInfoLog( obj, infologLength, &charsWritten, infoLog );
        printf( "%s\n", infoLog );
        free( infoLog );
    }
}
}

void printProgramInfoLog( GLuint obj )
{
    int infologLength = 0;
    int charsWritten  = 0;
    char* infoLog;

    glGetProgramiv( obj, GL_INFO_LOG_LENGTH, &infologLength );

    if ( infologLength > 0 )
    {
        infoLog = ( char* )malloc( infologLength );
        glGetProgramInfoLog( obj, infologLength, &charsWritten, infoLog );
        printf( "%s\n", infoLog );
        free( infoLog );
    }
}

Shader::Shader( std::string const& Name )
    : mVertexId( 0 )
    , mFragmentId( 0 )
    , mProgramId( 0 )
{
    Load( Name );
}

Shader::~Shader()
{
    Unload();
}

void Shader::Unbind()const
{
    glUseProgram( 0 );
}

void Shader::Load( std::string const& Name )
{
    Filesys& FSys = Filesys::Get();
    AutoFile Vs = FSys.Open( "shaders/" + Name + ".vs" );
    AutoFile Fs = FSys.Open( "shaders/" + Name + ".fs" );
    if( !Vs.get() || !Fs.get() )
    {
        assert( false );
        return;
    }
    mVertexId = glCreateShader( GL_VERTEX_SHADER );
    mFragmentId = glCreateShader( GL_FRAGMENT_SHADER );
    std::string Buf;
    if( !Vs->ReadAll( Buf ) )
    {
        Unload();
        assert( false );
        return;
    }
    char const* Ptr = Buf.c_str();
    glShaderSource( mVertexId, 1, &Ptr, 0 );
    glCompileShader( mVertexId );
    printShaderInfoLog( mVertexId );
    if( !Fs->ReadAll( Buf ) )
    {
        Unload();
        assert( false );
        return;
    }
    Ptr = Buf.c_str();
    glShaderSource( mFragmentId, 1, &Ptr, 0 );
    glCompileShader( mFragmentId );
    printShaderInfoLog( mFragmentId );
    mProgramId = glCreateProgram();
    glAttachShader( mProgramId, mFragmentId );
    glAttachShader( mProgramId, mVertexId );
    glLinkProgram( mProgramId );
    printProgramInfoLog( mProgramId );

    GLuint GlobalMatrices = glGetUniformBlockIndex( mProgramId, "GlobalMatrices" );
    if( GlobalMatrices != GL_INVALID_INDEX )
    {
        glUniformBlockBinding( mProgramId, GlobalMatrices, 0 );
    }
    else
    {
		L1( "  %s\n", Name );
        assert( false );
    }

    GLint n = 200;
    glGetProgramiv( mProgramId, GL_ACTIVE_UNIFORMS, &n );
    // get max uniform name length
    GLint max_name_length;
    glGetProgramiv( mProgramId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_length );
    // buffer for name
    GLchar* name = new GLchar[max_name_length];
    // buffer for length
    GLsizei length = 100;
    // Type of variable
    GLenum type;
    // Count of variables
    GLint size;
    // loop over active uniforms getting each's name
    L1( "Active uniforms in %u:\n", mProgramId );
    for( GLint u = 0; u < n; u++ )
    {
        glGetActiveUniform( mProgramId, u, max_name_length, &length, &size, &type, name );
        L1( "  %s\n", name );
    }
}

void Shader::Unload()
{
    Unbind();
    glDetachShader( mProgramId, mFragmentId );
    glDetachShader( mProgramId, mVertexId );
    glDeleteShader( mFragmentId );
    glDeleteShader( mVertexId );
    glDeleteProgram( mProgramId );
}

void Shader::Bind()const
{
    glUseProgram( mProgramId );
}

