#ifndef INCLUDED_RENDER_ACTOR_RENDERER_H
#define INCLUDED_RENDER_ACTOR_RENDERER_H
#include "platform/i_platform.h"
#include "core/scene.h"
#include "recognizer_repo.h"
#include "shader_manager.h"
#include "action_renderer.h"
#include "action_renderer_factory.h"
#include "renderable_sprite.h"
#include "core/actor_event.h"
#include "vao_base.h"
#include "input/mouse.h"
#include "counter.h"
#include "camera.h"
using render::RenderableSprite;
using render::RecognizerRepo;
using render::ActionRenderer;
using render::ActionRendererFactory;
class ActorRenderer
{
public:
    typedef ActionRenderer::RenderableSprites_t RenderableSprites_t;
    typedef boost::function<bool( IRenderableComponent const& )> RenderFilter;
    typedef std::function<glm::vec4( IRenderableComponent const& )> ColorFilter;
    typedef std::function<void(ShaderManager&)> SetupFunction;
private:
    void Init();
    void DrawOnePart( render::CountByTexId const& Part, size_t instances ) const;
    VaoBase mVAO;
    RecognizerRepo& mRecognizerRepo;
    ActionRendererFactory& mActionRendererFactory;
    AutoReg mOnActorEvent;
    void OnActorEvent( ActorEvent const& Evt );

    struct FindActionRenderer
    {
        int32_t mActionRendererId;
        FindActionRenderer( int32_t actionRendererId )
            : mActionRendererId( actionRendererId )
        {
        }
        bool operator()( const ActionRenderer& actionRenderer )
        {
            return actionRenderer.GetId() == mActionRendererId;
        }
    };
    typedef boost::ptr_set<ActionRenderer> ActionRenderers_t;
    typedef std::map<int, ActionRenderers_t> ActionRenderersMap_t;
    ActionRenderersMap_t mActionRenderersMap;
    void OnMouseMoveEvent( const WorldMouseMoveEvent& Event );
    AutoReg mMouseMoveId;
    size_t mPrevSize = 0;
    double mX = 0.0;
    double mY = 0.0;
    size_t mTexIndex = 0;
    size_t mSecondaryTexIndex = 0;
    size_t mPosIndex = 0;
    size_t mHeadingIndex = 0;
    size_t mSizeIndex = 0;
    size_t mColorIndex = 0;
    size_t mPostprocessColorIndex = 0;
    size_t mMaskTexCoordIndex = 0;
    size_t mNormalTexCoordIndex = 0;
    size_t mShadowLenScaleIndex = 0;
    std::map<int32_t,std::vector<glm::vec4> > mPostprocessColors;
    render::Counts_t mCounts;
    RenderableSprites_t mRenderableSprites;
public:
    ActorRenderer();
    ~ActorRenderer();
    void Prepare( Scene const& scene, std::vector<Camera const*> const& cameras, double deltaTime );
    void Draw( RenderFilter filter, SetupFunction setup = SetupFunction(), size_t extraInstances = 0 );
    void Draw( int32_t postprocess );
};

#endif//INCLUDED_RENDER_ACTOR_RENDERER_H
