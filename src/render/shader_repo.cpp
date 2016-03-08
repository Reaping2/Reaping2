#include "i_render.h"
#include "shader_repo.h"
#include "shader.h"

ShaderRepo::ShaderRepo()
    : Repository<Shader>( mDefaultShader )
    , mDefaultShader( mDefaultShaderName )
{
    LoadShaders();
}

void ShaderRepo::LoadShaders()
{
    Filesys& FSys = Filesys::Get();
    PathVect_t Paths;
    FSys.GetFileNames( Paths, "shaders" );
    std::set<std::string> Loaded;
    Loaded.insert( mDefaultShaderName );
    for( PathVect_t::const_iterator i = Paths.begin(), e = Paths.end(); i != e; ++i )
    {
        boost::filesystem::path Path = *i;
        Path = Path.filename();
        Path.replace_extension();
        std::string const& ShaderName = Path.string();
        if( !Loaded.insert( ShaderName ).second )
        {
            continue;
        }
        int32_t Idx = AutoId( ShaderName );
        mElements.insert( Idx, new Shader( ShaderName ) );
    }
}

std::string const ShaderRepo::mDefaultShaderName = "default";

