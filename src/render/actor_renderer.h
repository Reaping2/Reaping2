#ifndef INCLUDED_RENDER_ACTOR_RENDERER_H
#define INCLUDED_RENDER_ACTOR_RENDERER_H
#include "platform/i_platform.h"
#include "core/scene.h"
#include "recognizer_repo.h"
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
private:
    void Init();
    void DrawOnePart( render::CountByTexId const& Part ) const;
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
    size_t mPrevSize;
    double mX;
    double mY;
    size_t mTexIndex;
    size_t mSecondaryTexIndex;
    size_t mPosIndex;
    size_t mHeadingIndex;
    size_t mSizeIndex;
    size_t mColorIndex;
    size_t mPostprocessColorIndex;
    size_t mMaskTexCoordIndex;
    size_t mNormalTexCoordIndex;
    std::map<int32_t,std::vector<glm::vec4> > mPostprocessColors;
    render::Counts_t mCounts;
    RenderableSprites_t mRenderableSprites;
public:
    ActorRenderer();
    ~ActorRenderer();
    void Prepare( Scene const& scene, Camera const& camera, double deltaTime );
    void Draw( RenderFilter filter );
    void Draw( int32_t postprocess );
};

#endif//INCLUDED_RENDER_ACTOR_RENDERER_H
