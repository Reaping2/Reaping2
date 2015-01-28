#include "render/action_renderer.h"
namespace render{


    ActionRenderer::~ActionRenderer()
    {

    }

    ActionRenderer::ActionRenderer(int32_t Id)
        : mId(Id)
        , mSecsToEnd(1.0)
        , mState(100)
        , mRenderableRepo(RenderableRepo::Get())
        , mOrder(0)
    {

    }

    void ActionRenderer::Init(const Actor& actor)
    {
//         SpriteCollection const& Sprites=mRenderableRepo(actor.GetId());
//         Sprites();
    }

    void ActionRenderer::FillRenderableSprites(const Actor& actor, RenderableSprites_t& renderableSprites)
    {

    }

    void ActionRenderer::Update(double DeltaTime)
    {
        double nextState = mSecsToEnd==0?100:(mState + 1. / mSecsToEnd * DeltaTime * 100.);

        if( nextState >= 100 )
        {
//             if( mIsLoop )
//             {
                nextState = fmod( nextState, 100. );
//            }
//             else
//             {
//                 nextState = 100.;
//             }
        }
        mState = nextState;
    }

    bool ActionRenderer::operator<(const render::ActionRenderer& r) const
    {
        return mOrder<r.GetOrder();
    }


    DefaultActionRenderer::DefaultActionRenderer(int32_t Id)
        : ActionRenderer(Id)
    {

    }

} // namespace render