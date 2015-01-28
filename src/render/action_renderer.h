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
        ActionRenderer( int32_t Id );
        virtual void Update( double DeltaTime );
        virtual void Init(const Actor& actor);
        virtual void FillRenderableSprites(const Actor& actor, RenderableSprites_t& renderableSprites);
    protected:
        int32_t mId;
        double mSecsToEnd;                          // speed. This much seconds is needed, till this action counts from 0..100.
        double mState;

        RenderableRepo& mRenderableRepo;
        int32_t mOrder;
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
        void SetOrder(int32_t order)
        {
            mOrder=order;
        }
        bool operator<( const render::ActionRenderer& r ) const;
    };

    class DefaultActionRenderer : public ActionRenderer
    {
    public:
        DefaultActionRenderer( int32_t Id );
    };


} // namespace render
#endif //INCLUDED_RENDER_ACTION_RENDERER_H
