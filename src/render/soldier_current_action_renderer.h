#ifndef INCLUDED_RENDER_SOLDIER_CURRENT_ACTION_RENDERER_H
#define INCLUDED_RENDER_SOLDIER_CURRENT_ACTION_RENDERER_H

#include "platform/i_platform.h"
#include "render/action_renderer.h"
#include "core/actor.h"
#include "renderable_sprite.h"
#include "renderable_repo.h"
#include "hat_action_renderer.h"

namespace render {
namespace ctf {

class SoldierCurrentActionRenderer : public ActionRenderer
{
    ColorRepo& mColorRepo;
    glm::vec4 mColor;
public:
    SoldierCurrentActionRenderer( int32_t Id );
    virtual void FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites );
protected:
    virtual glm::vec4 GetRenderableColor( Actor const& actor ) const;
};

} // namespace ctf
} // namespace render

#endif//INCLUDED_RENDER_SOLDIER_CURRENT_ACTION_RENDERER_H

//command:  "classgenerator.exe" -g "action_renderer" -c "ctf_hat_action_renderer"
