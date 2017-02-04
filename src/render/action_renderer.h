#ifndef INCLUDED_RENDER_ACTION_RENDERER_H
#define INCLUDED_RENDER_ACTION_RENDERER_H
#include "platform/i_platform.h"
#include "core/actor.h"
#include "renderable_sprite.h"
#include "renderable_repo.h"
#include "sprite_collection.h"
#include "action_renderer_loader.h"

namespace render {

struct CompanionSprites
{
    Sprite const* MaskSpr = nullptr;
    Sprite const* NormalSpr = nullptr;
    std::vector<Sprite const*> AdditionalSprs;
};
class ActionRenderer
{
public:
    typedef std::vector<RenderableSprite> RenderableSprites_t;
    virtual ~ActionRenderer();
    ActionRenderer( int32_t Id, int32_t ActionId = -1 );
    virtual void Update( double DeltaTime );
    virtual void Init( const Actor& actor );
    virtual void FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites );
protected:
    virtual glm::vec4 GetRenderableColor( Actor const& actor ) const;
    int32_t mId = -1;        // id of the renderer
    int32_t mActionId = -1;  // id of the action (e.g. AutoId( "idle" ))
    double mSecsToEnd = 1.0;                          // speed. This much seconds is needed, till this action counts from 0..100.
    double mState = 0.0;

    Sprite const* mSpr = nullptr;
    CompanionSprites mCompanionSprites;

    RenderableRepo& mRenderableRepo;
    int32_t mOrder = -1;
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
int32_t GetSpriteId( int32_t spriteIndex, int32_t actorId );
CompanionSprites GetCompanionSprites( int32_t actorId, int32_t actionId );
void FillRenderableSprite( RenderableSprite& rs, CompanionSprites const& data, int32_t state );
} // namespace render
#endif //INCLUDED_RENDER_ACTION_RENDERER_H
