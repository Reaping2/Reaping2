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
using render::RenderableSprite;
using render::RecognizerRepo;
using render::ActionRenderer;
using render::ActionRendererFactory;
class ActorRenderer
{
    struct CountByTexId
    {
        GLuint TexId;
        size_t Start;
        size_t Count;
        CountByTexId( GLuint t, size_t s, size_t c ): TexId( t ), Start( s ), Count( c ) {}
    };
    typedef std::vector<CountByTexId> Counts_t;

    struct RenderableSpriteCompare
    {
        bool operator()( RenderableSprite const& Rs1, RenderableSprite const& Rs2 );
    };

    void Init();
    VaoBase mVAO;
    RecognizerRepo& mRecognizerRepo;
    ActionRendererFactory& mActionRendererFactory;
    AutoReg mOnActorEvent;
    void OnActorEvent( ActorEvent const& Evt );

    struct FindActionRenderer
    {
        int32_t mActionRendererId;
        FindActionRenderer(int32_t actionRendererId)
            : mActionRendererId(actionRendererId)
        {
        }
        bool operator()(const ActionRenderer& actionRenderer)
        {
            return actionRenderer.GetId()==mActionRendererId;
        }
    };
    typedef boost::ptr_set<ActionRenderer> ActionRenderers_t;
    typedef std::map<int,ActionRenderers_t> ActionRenderersMap_t;
    ActionRenderersMap_t mActionRenderersMap;
public:
    ActorRenderer();
    ~ActorRenderer();
    void Draw( Scene const& Object, double DeltaTime );
};

#endif//INCLUDED_RENDER_ACTOR_RENDERER_H
