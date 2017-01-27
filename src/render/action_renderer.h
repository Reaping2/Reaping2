#ifndef INCLUDED_RENDER_ACTION_RENDERER_H
#define INCLUDED_RENDER_ACTION_RENDERER_H
#include "platform/i_platform.h"
#include "core/actor.h"
#include "renderable_sprite.h"
#include "renderable_repo.h"
#include "sprite_collection.h"

namespace render {

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
    Sprite const* mMaskSpr = nullptr;
    Sprite const* mNormalSpr = nullptr;
    std::vector<Sprite const*> mAdditionalSprs;

    RenderableRepo& mRenderableRepo;
    int32_t mOrder = -1;
public:
    double GetState()const
    {
        return mState;
    }
    void SetState( double S )
    {
        mState = S;
    }
    int32_t GetId() const
    {
        return mId;
    }
    int32_t GetOrder() const
    {
        return mOrder;
    }
    void SetOrder( int32_t order )
    {
        mOrder = order;
    }
    bool operator<( const render::ActionRenderer& r ) const;
};

class DefaultActionRenderer : public ActionRenderer
{
public:
    DefaultActionRenderer( int32_t Id );
};

glm::vec4 GetCloakColor( const Actor& actor );
glm::vec4 GetColor( const Actor& actor );
int32_t GetSpriteId( int32_t spriteIndex, int32_t actorId );
struct CompanionSprites
{
    Sprite const* MaskSpr = nullptr;
    Sprite const* NormalSpr = nullptr;
    std::vector<Sprite const*> AdditionalSprs;
};
CompanionSprites GetCompanionSprites( int32_t actorId, int32_t actionId );
} // namespace render
#endif //INCLUDED_RENDER_ACTION_RENDERER_H
