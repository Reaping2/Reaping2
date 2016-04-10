#ifndef INCLUDED_RENDER_HEALTH_BAR_RENDERER_H
#define INCLUDED_RENDER_HEALTH_BAR_RENDERER_H
#include "core/scene.h"
#include "recognizer_repo.h"
#include "action_renderer.h"
#include "action_renderer_factory.h"
#include "renderable_sprite.h"
#include "core/actor_event.h"
#include "ui_renderer.h"
#include "text.h"
#include "core/program_state.h"
#include "hat_action_renderer.h"
#include "core/settings.h"

struct SceneVertex
{
    glm::vec2 Pos;
    glm::vec2 TexCoord;
    glm::vec4 Color;
    GLuint TexId;
    glm::vec2 RealPos;
    SceneVertex( glm::vec2 const& p, glm::vec2 const& t, glm::vec4 const& c, GLuint tid, glm::vec2 rp )
        : Pos( p ), TexCoord( t ), Color( c ), TexId( tid ), RealPos( rp ) {}
    SceneVertex( glm::vec2 const& p, glm::vec4 const& c, glm::vec2 rp )
        : Pos( p ), Color( c ), TexCoord( -1. ), TexId( 0xffffffff ), RealPos( rp ) {}
};
typedef std::vector<SceneVertex> SceneVertices_t;
typedef std::back_insert_iterator<SceneVertices_t> SceneVertexInserter_t;
class HealthBarRenderer
{
    SceneVertices_t mPrevVertices;
    VaoBase mVAO;
    Opt<engine::WindowSystem> mWindow;
    typedef std::vector<Text> Texts_t;
    Texts_t mTexts;
    core::ProgramState& mProgramState;
    render::ColorRepo& mColorRepo;
    Settings& mSettings;
    glm::vec2 mSize;
    glm::vec2 mBorderSize;
    glm::vec2 mPosition;
    glm::vec2 mBorderPosition;
    void Init();
public:
    HealthBarRenderer();
    void Draw();
    void AddText( Text const& text );
    static void ColoredBox( glm::vec4 const& Dim, glm::vec4 const& Col, SceneVertexInserter_t& Inserter );
};

#endif//INCLUDED_RENDER_HEALTH_BAR_RENDERER_H
