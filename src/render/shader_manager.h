#ifndef INCLUDED_RENDER_SHADER_MANAGER_H
#define INCLUDED_RENDER_SHADER_MANAGER_H
#include "platform/i_platform.h"
#include "shader.h"

struct GlobalShaderData
{
    enum Type
    {
        WorldProjection,
        WorldCamera,
        InverseProjection,
        Resolution,
        // keep at end
        TotalSize,
        NumData,
    };
};

class ShaderManager : public Singleton<ShaderManager>
{
    GLuint mGlobalsUBO;
    size_t mGlobalOffsets[GlobalShaderData::NumData];
    void InitGlobalUniforms();
    friend class Singleton<ShaderManager>;
    ShaderManager();
    Shader const* mActiveShader;
    int32_t mActiveShaderId = -1;
    Shader const* mDefaultShader = nullptr;
    int32_t mDefaultShaderId = -1;
    typedef std::map<std::string, GLuint> LocMap_t;
    typedef std::map<GLuint, LocMap_t> ShaderMap_t;
    ShaderMap_t mShaderLocs;
    GLuint GetUniformLocation( std::string const& Loc );
public:
    ~ShaderManager();
    template<typename T>
    void UploadGlobalData( GlobalShaderData::Type GlobalType, T const& Mat )const;
    void ActivateShader( int32_t id );
    void SetDefaultShader( int32_t id );
    template<typename T>
    void UploadData( std::string const& Name, T const& Data );
};

template<>
void ShaderManager::UploadData( std::string const& Name, GLuint const& Data );

template<>
void ShaderManager::UploadData( std::string const& Name, GLfloat const& Data );

template<>
void ShaderManager::UploadData( std::string const& Name, glm::vec2 const& Data );

template<>
void ShaderManager::UploadData( std::string const& Name, glm::mat2 const& Data );

template<>
void ShaderManager::UploadData( std::string const& Name, glm::mat4 const& Data );


template<typename T>
void ShaderManager::UploadData( std::string const& Name, T const& Data )
{
    assert( false );
}

template<typename T>
void ShaderManager::UploadGlobalData( GlobalShaderData::Type GlobalType, T const& Mat ) const
{
    glBindBuffer( GL_UNIFORM_BUFFER, mGlobalsUBO );
    glBufferSubData( GL_UNIFORM_BUFFER, mGlobalOffsets[GlobalType], sizeof( T ), glm::value_ptr( Mat ) );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );
}

#endif//INCLUDED_RENDER_SHADER_MANAGER_H
