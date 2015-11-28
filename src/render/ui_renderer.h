#ifndef INCLUDED_RENDER_UI_RENDERER_H
#define INCLUDED_RENDER_UI_RENDERER_H
#include "platform/i_platform.h"
#include "ui/i_ui.h"

#include "core/opt.h"
#include "vao_base.h"

struct UiVertex
{
    glm::vec2 Pos;
    glm::vec2 TexCoord;
    glm::vec4 Color;
    GLuint TexId;
    UiVertex( glm::vec2 const& p, glm::vec2 const& t, glm::vec4 const& c, GLuint tid )
        : Pos( p ), TexCoord( t ), Color( c ), TexId( tid ) {}
    UiVertex( glm::vec2 const& p, glm::vec4 const& c )
        : Pos( p ), Color( c ), TexCoord( -1. ), TexId( 0xffffffff ) {}
};
typedef std::vector<UiVertex> UiVertices_t;
typedef std::back_insert_iterator<UiVertices_t> UiVertexInserter_t;
class UiRenderer
{
    UiVertices_t mPrevVertices;
    VaoBase mVAO;
    Opt<engine::WindowSystem> mWindow;
    void Init();
public:
    UiRenderer();
    void Draw( Root const& UiRoot, const glm::mat4& projMatrix );
};

#endif//INCLUDED_RENDER_UI_RENDERER_H
