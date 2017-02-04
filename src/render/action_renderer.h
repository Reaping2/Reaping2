#ifndef INCLUDED_RENDER_ACTION_RENDERER_H
#define INCLUDED_RENDER_ACTION_RENDERER_H
#include "platform/i_platform.h"
#include "core/actor.h"
#include "renderable_sprite.h"
#include "renderable_repo.h"
#include "sprite_collection.h"
#include "action_renderer_loader.h"

namespace render {

class ActionRenderer
{
public:
    typedef std::vector<RenderableSprite> RenderableSprites_t;
    virtual ~ActionRenderer();
    ActionRenderer( int32_t Id );
    virtual void Update( double DeltaTime );
    virtual void Init( const Actor& actor );
    virtual void FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites );
protected:
    int32_t mId;
    double mSecsToEnd;                          // speed. This much seconds is needed, till this action counts from 0..100.
    double mState;

    RenderableRepo& mRenderableRepo;
    int32_t mOrder;
public:
    double GetState()const;
    void SetState( double S );
    int32_t GetId() const;
    int32_t GetOrder() const;
    void SetOrder( int32_t order );
    bool operator<( const render::ActionRenderer& r ) const;
};

class DefaultActionRenderer : public ActionRenderer
{
public:
    DefaultActionRenderer( int32_t Id );
};

class DefaultActionRendererLoader : public ActionRendererLoader<DefaultActionRenderer>
{
public:
    virtual void BindValues();
    DefaultActionRendererLoader() = default;
};

glm::vec4 GetCloakColor( const Actor& actor );
glm::vec4 GetColor( const Actor& actor );
} // namespace render
#endif //INCLUDED_RENDER_ACTION_RENDERER_H
