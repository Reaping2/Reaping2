#include "i_render.h"
#include "texture_repo.h"
#include "texture.h"

void TextureRepo::Init()
{
    PathVect_t Paths;
    mFilesys.GetFileNames( Paths, "textures" );
    for( PathVect_t::const_iterator i = Paths.begin(), e = Paths.end(); i != e; ++i )
    {
        boost::filesystem::path const& Path = *i;
        if( Path.extension().string() != ".png" )
        {
            continue;
        }
        AutoFile TexFile = mFilesys.Open( Path );
        if( !TexFile.get() )
        {
            continue;
        }
        PngTexture Png( *TexFile );
        if( !Png.IsValid() )
        {
            continue;
        }
        Texture* Tex = new Texture( Png.GetWidth(), Png.GetHeight(), Png.GetChannels(), Png.GetData() );
        int32_t Id = AutoId( Path.generic_string() );
        mElements.insert( Id, Tex );
    }
}

TextureRepo::TextureRepo()
    : RepoBase( mDefaultTexture )
    , mDefaultTexture( 1, 1, 0, NULL )
    , mFilesys( Filesys::Get() )
{
    // Init();
}

Texture& TextureRepo::operator()( int32_t Id )
{
    ElementMap_t::iterator i = mElements.find( Id );
    if( i != mElements.end() )
    {
        return *( i->second );
    }
    if( std::find( mUnavailElements.begin(), mUnavailElements.end(), Id ) != mUnavailElements.end() )
    {
        return const_cast<Texture&>(mDefaultElement);
    }
    do
    {
        std::string Path;
        if( !IdStorage::Get().GetName( Id, Path ) )
        {
            break;
        }
        AutoFile TexFile = mFilesys.Open( Path );
        if( !TexFile.get() )
        {
            break;
        }
        std::auto_ptr<TextureBase> TexBase;
        if( boost::iequals( boost::filesystem::path( Path ).extension().string(), ".png" ) )
        {
            TexBase.reset( new PngTexture( *TexFile ) );
        }
        else if( boost::iequals( boost::filesystem::path( Path ).extension().string(), ".tga" ) )
        {
            TexBase.reset( new TgaTexture( *TexFile ) );
        }
        if( !TexBase.get() )
        {
            break;
        }
        std::auto_ptr<Texture> Tex( new Texture( TexBase->GetWidth(), TexBase->GetHeight(), TexBase->GetChannels(), TexBase->GetData() ) );
        if( Tex->TexId() == 0 )
        {
            break;
        }
        mElements.insert( Id, Tex.get() );
        return *Tex.release();
    }
    while( false );
    mUnavailElements.push_back( Id );
    return const_cast<Texture&>(mDefaultElement);
}

