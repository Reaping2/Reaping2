#ifndef INCLUDED_RENDER_RENDERABLE_REPO_H
#define INCLUDED_RENDER_RENDERABLE_REPO_H
#include "platform/i_platform.h"
#include "sprite_collection.h"

//class SpriteCollection;
class RenderableRepo : public Repository<SpriteCollection>, public Singleton<RenderableRepo>
{
    SpriteCollection mDefaultRenderable;
    std::map<int32_t,float> mMaxScaleMap;
    friend class Singleton<RenderableRepo>;
    RenderableRepo();
    void Init();
public:
    float GetMaxScale( int32_t actorId ) const;
};

#endif//INCLUDED_RENDER_RENDERABLE_REPO_H
