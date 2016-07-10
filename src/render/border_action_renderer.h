#ifndef INCLUDED_RENDER_BORDER_ACTION_RENDERER_H
#define INCLUDED_RENDER_BORDER_ACTION_RENDERER_H

#include "platform/i_platform.h"
#include "render/action_renderer.h"
#include "core/actor.h"
#include "renderable_sprite.h"
#include "renderable_repo.h"
#include "core/border_type.h"
#include "core/i_border_component.h"

namespace render {

class BorderActionRenderer : public ActionRenderer
{
    typedef std::vector<int32_t> BorderIds_t;
    BorderIds_t mBorderIds;
    BorderIds_t mOuterBorderIds;
    int32_t mActionId;
    double mActorSize;
    BorderType& mBorderType;
    IBorderComponent::Borders_t mBorders;
    IBorderComponent::Borders_t mOuterBorders;
public:
    BorderActionRenderer( int32_t Id );
    virtual void Init( const Actor& actor );
    virtual void FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites );
private:
};

} // namespace render

#endif//INCLUDED_RENDER_BORDER_ACTION_RENDERER_H

//command:  "classgenerator" -g "action_renderer" -c "border_action_renderer"
