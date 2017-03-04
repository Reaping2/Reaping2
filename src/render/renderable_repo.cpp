#include "i_render.h"
#include "renderable_repo.h"
#include "sprite_collection.h"
#include "platform/filesystem_utils.h"

RenderableRepo::RenderableRepo()
    : RepoBase( mDefaultRenderable )
{
    Init();
}

void RenderableRepo::Init()
{
    ElementMap_t Renderables;
    fs_utils::for_each( "actors", ".sprite", [&]( Json::Value const& desc, boost::filesystem::path const& path )
    {
        std::string PathStr;
        auto const& ActorVisuals = desc["actor_visuals"];
        if (!ActorVisuals.isArray())
        {
            return;
        }
        if (!Json::GetStr( desc["texture_path"], PathStr ))
        {
            return;
        }
        bool isAbsolute = PathStr.size() > 1 && (PathStr.substr( 0, 2 ) == ":/" || PathStr.substr( 0, 2 ) == ":\\");
        boost::filesystem::path newPath;
        if (!isAbsolute)
        {
            L2( "Path is not absolute %s, %s\n", path.generic_string().c_str(), PathStr.c_str() );
            newPath = path.parent_path() / PathStr;
        }
        else
        {
            newPath = boost::filesystem::path( PathStr.substr( 2 ) );
            L2( "Path is absolute %s\n", newPath.generic_string().c_str() );
        }
        int32_t TexId = AutoId( newPath.generic_string() );
        for (auto&& ActorVisualDesc : ActorVisuals)
        {
            if (!ActorVisualDesc.isObject())
            {
                return;
            }
            std::auto_ptr<SpriteCollection> Renderable( new SpriteCollection );
            if (!Renderable->Load( TexId, ActorVisualDesc ))
            {
                return;
            }
            int32_t Id = Renderable->Id();
            ElementMap_t::iterator It = Renderables.find( Id );
            if (It == Renderables.end())
            {
                Renderables.insert( Id, Renderable.release() );
            }
            else
            {
                It->second->Merge( *Renderable );
            }
        }
    } );
    for( auto i = Renderables.begin(), e = Renderables.end(); i != e; ++i )
    {
        auto const& spriteColl = *i->second;
        float max = 0.0f;
        for( auto ii = spriteColl.begin(), ee = spriteColl.end(); ii != ee; ++ii)
        {
            if( max < ii->second->GetScale() )
            {
                max = ii->second->GetScale();
            }
        }
        mMaxScaleMap[i->first] = max;
    }
    // all done
    using std::swap;
    swap( mElements, Renderables );
}

float RenderableRepo::GetMaxScale( int32_t actorId ) const
{
    auto it = mMaxScaleMap.find( actorId );
    return it == mMaxScaleMap.end() ? 1.0 : it->second;
}

