#include "i_render.h"
#include "renderable_repo.h"
#include "sprite_collection.h"

RenderableRepo::RenderableRepo()
    : RepoBase( mDefaultRenderable )
{
    Init();
}

bool RenderableRepo::AddSpritesFromOneTextureDesc( Json::Value& TexDesc, ElementMap_t& Renderables, boost::filesystem::path const& parentPath )
{
    std::string PathStr;
    Json::Value& ActorVisuals = TexDesc["actor_visuals"];
    if( !ActorVisuals.isArray() )
    {
        return false;
    }
    if( !Json::GetStr( TexDesc["texture_path"], PathStr ) )
    {
        return false;
    }
    auto path = boost::filesystem::path( PathStr );
    if (!path.has_parent_path())
    {
        path = parentPath / PathStr;
    }
    int32_t TexId = AutoId( path.generic_string() );
    for( Json::Value::iterator i = ActorVisuals.begin(), e = ActorVisuals.end(); i != e; ++i )
    {
        Json::Value& ActorVisualDesc = *i;
        if( !ActorVisualDesc.isObject() )
        {
            return false;
        }
        std::auto_ptr<SpriteCollection> Renderable( new SpriteCollection );
        if( !Renderable->Load( TexId, ActorVisualDesc ) )
        {
            return false;
        }
        int32_t Id = Renderable->Id();
        ElementMap_t::iterator It = Renderables.find( Id );
        if( It == Renderables.end() )
        {
            Renderables.insert( Id, Renderable.release() );
        }
        else
        {
            It->second->Merge( *Renderable );
        }
    }
    return true;
}

void RenderableRepo::Init()
{
    PathVect_t Paths;
    Filesys& FSys = Filesys::Get();
    FSys.GetFileNames( Paths, "sprites" );
    ElementMap_t Renderables;
    for( PathVect_t::const_iterator i = Paths.begin(), e = Paths.end(); i != e; ++i )
    {
        boost::filesystem::path const& Path = *i;
        if( Path.extension().string() != ".json" )
        {
            continue;
        }
        AutoFile JsonFile = FSys.Open( *i );
        if( !JsonFile.get() )
        {
            continue;
        }
        JsonReader Reader( *JsonFile );
        if( !Reader.IsValid() )
        {
            continue;
        }
        Json::Value Root = Reader.GetRoot();
        if( !Root.isArray() )
        {
            continue;
        }
        for( Json::Value::iterator i = Root.begin(), e = Root.end(); i != e; ++i )
        {
            Json::Value& TexDesc = *i;
            if( !AddSpritesFromOneTextureDesc( TexDesc, Renderables, Path.parent_path() ) )
            {
                return;
            }
        }
    }
    // all done
    using std::swap;
    swap( mElements, Renderables );
}

