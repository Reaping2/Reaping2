#ifndef INCLUDED_RENDER_NAME_RENDERER_H
#define INCLUDED_RENDER_NAME_RENDERER_H
#include "core/scene.h"
#include "recognizer_repo.h"
#include "action_renderer.h"
#include "action_renderer_factory.h"
#include "renderable_sprite.h"
#include "core/actor_event.h"
#include "ui_renderer.h"
#include "text_scene_renderer.h"
#include "core/program_state.h"
#include "hat_action_renderer.h"
#include "core/settings.h"

class NameRenderer
{
    void Init();
    core::ProgramState& mProgramState;
    render::ColorRepo& mColorRepo;
    Settings& mSettings;
    int32_t mSize;
    int32_t mY;
public:
    NameRenderer();
    void Draw( TextSceneRenderer& textSceneRenderer );
};

#endif//INCLUDED_RENDER_NAME_RENDERER_H
