#ifndef INCLUDED_RENDER_RENDERER_H
#define INCLUDED_RENDER_RENDERER_H
#include "platform/i_platform.h"
#include "render/i_render.h"
#include "name_renderer.h"
#include "text_scene_renderer.h"
#include "camera.h"
#include "projection.h"
#include "actor_renderer.h"
#include "decal_engine.h"
#include "shader_manager.h"
#include "health_bar_renderer.h"
#include "mouse_renderer.h"
#include "world_renderer.h"
#include "core/perf_timer.h"
#include "path_box_renderer.h"

namespace engine {
class RendererSystem : public System
{
    ~RendererSystem();

    Projection mWorldProjector;
    Projection mUiProjector;
    Camera mCamera;
    Ui& mUi;
    ActorRenderer mActorRenderer;
    UiRenderer mUiRenderer;
    NameRenderer mNameRenderer;
    PathBoxRenderer mPathBoxRenderer;
    TextSceneRenderer mTextSceneRenderer;
    HealthBarRenderer mHealthBarRenderer;
    MouseRenderer mMouseRenderer;
    render::WorldRenderer mWorldRenderer;
    DecalEngine& mDecalEngine;
    ShaderManager& mShaderManager;
    AutoReg mMouseMoveId;
    AutoReg mMousePressId;
    AutoReg mMouseReleaseId;

    glm::vec3 mMouseRawPos;
    glm::vec3 mMouseWorldPos;

    void SetupRenderer( const Projection& Proj, float Scale = 1.0f );
    void SetupIdentity();

    void OnMouseMoveEvent( const ScreenMouseMoveEvent& Event );
    void OnMousePressEvent( const ScreenMousePressEvent& Event );
    void OnMouseReleaseEvent( const ScreenMouseReleaseEvent& Event );

public:
    RendererSystem();
    DEFINE_SYSTEM_BASE( RendererSystem )
    virtual void Init();
    virtual void Update( double DeltaTime );

    void SendWorldMouseMoveEvent();
    Camera const& GetCamera() const;
    perf::Timer_t mPerfTimer;
};
} // namespace engine

#endif//INCLUDED_RENDER_RENDERER_H

