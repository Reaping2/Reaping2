#ifndef INCLUDED_RENDER_TEXT_SCENE_RENDERER_H
#define INCLUDED_RENDER_TEXT_SCENE_RENDERER_H
#include "core/scene.h"
#include "recognizer_repo.h"
#include "action_renderer.h"
#include "action_renderer_factory.h"
#include "renderable_sprite.h"
#include "core/actor_event.h"
#include "ui_renderer.h"
#include "text.h"

class TextSceneRenderer
{
    UiVertices_t mPrevVertices;
    VaoBase mVAO;
    Opt<engine::WindowSystem> mWindow;
    typedef std::vector<Text> Texts_t;
    Texts_t mTexts;
    void Init();
public:
    TextSceneRenderer();
    void Draw();
    void AddText(Text const& text);
};

#endif//INCLUDED_RENDER_TEXT_SCENE_RENDERER_H
