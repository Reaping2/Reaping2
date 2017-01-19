#ifndef INCLUDED_RENDER_RENDERABLE_REPO_H
#define INCLUDED_RENDER_RENDERABLE_REPO_H
#include "platform/i_platform.h"
#include "sprite_collection.h"

//class SpriteCollection;
class RenderableRepo : public Repository<SpriteCollection>, public Singleton<RenderableRepo>
{
    SpriteCollection mDefaultRenderable;
    friend class Singleton<RenderableRepo>;
    RenderableRepo();
    void Init();
    bool AddSpritesFromOneTextureDesc( Json::Value& TexDesc, ElementMap_t& Renderables, boost::filesystem::path const& parentPath );
};

#endif//INCLUDED_RENDER_RENDERABLE_REPO_H
