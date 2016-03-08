#ifndef INCLUDED_RENDER_HAT_ACTION_RENDERER_H
#define INCLUDED_RENDER_HAT_ACTION_RENDERER_H
#include "platform/i_platform.h"
#include "core/actor.h"
#include "renderable_sprite.h"
#include "renderable_repo.h"
#include "action_renderer.h"
#include <map>
#include "platform/repository.h"

namespace render {

class ColorRepo : public platform::Repository<glm::vec4>, public platform::Singleton<ColorRepo>
{
    friend class Singleton<ColorRepo>;
    static glm::vec4 const mDefaultColor;
    ColorRepo();
};

class HatActionRenderer: public ActionRenderer
{
    int32_t mHatId;
public:
    HatActionRenderer( int32_t Id );
    virtual void Init( const Actor& actor );
    virtual void FillRenderableSprites( const Actor& actor, RenderableSprites_t& renderableSprites );
};


} // namespace render

#endif //INCLUDED_RENDER_HAT_ACTION_RENDERER_H