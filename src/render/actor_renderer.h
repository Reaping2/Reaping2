#ifndef INCLUDED_RENDER_ACTOR_RENDERER_H
#define INCLUDED_RENDER_ACTOR_RENDERER_H
#include "core/scene.h"
#include "recognizer_repo.h"
using render::RecognizerRepo;
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
    struct RenderableSprite
    {
        Actor const* Obj;
        int32_t ActId;
        SpritePhase const* Spr;
        Sprite const* Anim;
        RenderableSprite( Actor const* o, int32_t a, Sprite const* s, SpritePhase const* p )
            : Obj( o ), ActId( a ), Anim( s ), Spr( p ) {}
    };

    struct RenderableSpriteCompare
    {
        bool operator()( RenderableSprite const& Rs1, RenderableSprite const& Rs2 );
    };

    void Init();
    VaoBase mVAO;
    RecognizerRepo& mRecognizerRepo;
public:
    ActorRenderer();
    ~ActorRenderer();
    void Draw( Scene const& Object );
};

#endif//INCLUDED_RENDER_ACTOR_RENDERER_H
