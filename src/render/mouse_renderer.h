#ifndef INCLUDED_RENDER_MOUSE_RENDERER_H
#define INCLUDED_RENDER_MOUSE_RENDERER_H
#include "platform/i_platform.h"
#include "core/scene.h"
#include "recognizer_repo.h"
#include "action_renderer.h"
#include "action_renderer_factory.h"
#include "renderable_sprite.h"
#include "core/actor_event.h"
#include "vao_base.h"
#include "input/mouse.h"
#include "text_scene_renderer.h"
#include "core/program_state.h"
using render::RenderableSprite;
class MouseRenderer
{
    void Init();
    VaoBase mVAO;
    void OnMouseMoveEvent( const WorldMouseMoveEvent& Event );
    AutoReg mMouseMoveId;
    double mX;
    double mY;
    Scene& mScene;
    core::ProgramState& mProgramState;
public:
    MouseRenderer();
    ~MouseRenderer();
    void Draw( TextSceneRenderer& textSceneRenderer );

};

#endif//INCLUDED_RENDER_MOUSE_RENDERER_H
