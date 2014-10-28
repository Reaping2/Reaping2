#ifndef INCLUDED_RENDER_RENDERABLE_REPO_H
#define INCLUDED_RENDER_RENDERABLE_REPO_H

class RenderableRepo : public Repository<SpriteCollection>, public Singleton<RenderableRepo>
{
    static SpriteCollection DefaultRenderable;
    friend class Singleton<RenderableRepo>;
    RenderableRepo();
    void Init();
    bool AddSpritesFromOneTextureDesc( Json::Value& TexDesc, ElementMap_t& Renderables );
};

#endif//INCLUDED_RENDER_RENDERABLE_REPO_H
