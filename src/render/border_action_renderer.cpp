#include "render/border_action_renderer.h"
#include "core/border_type.h"
#include "core/i_border_component.h"
#include "platform/id_storage.h"

namespace render {

BorderActionRenderer::BorderActionRenderer(int32_t Id)
    : ActionRenderer(Id)
    , mActionId(AutoId("body_idle"))
{
}


void BorderActionRenderer::Init(Actor const& actor)
{
    static BorderType& mBorderType=BorderType::Get();
    static IdStorage& mIdStorage=IdStorage::Get();
    Opt<IBorderComponent> borderC=actor.Get<IBorderComponent>();
    if (!borderC.IsValid())
    {
        // this should be valid. border_recognizer guarantees it. Other recognizers can be paired with this renderer
        BOOST_ASSERT(borderC.IsValid());
        return;
    }
    std::string actorName;
    mIdStorage.GetName(actor.GetId(),actorName);
    IBorderComponent::Borders_t borders=borderC->GetBorders();
    for (IBorderComponent::Borders_t::iterator i=borders.begin(),e=borders.end();i!=e;++i)
    {
        std::string borderName;
        if(mIdStorage.GetName(mBorderType(*i),borderName))
        {
            mBorderIds.push_back(mIdStorage.GetId(actorName+"_"+borderName));
        }
    }

}


void BorderActionRenderer::FillRenderableSprites(const Actor& actor, RenderableSprites_t& renderableSprites)
{
    for (BorderIds_t::iterator i=mBorderIds.begin(),e=mBorderIds.end();i!=e;++i)
    {
        SpriteCollection const& Sprites=mRenderableRepo(*i);
        Sprite const& Spr=Sprites(mActionId);
        if( Spr.IsValid() )
        {
            SpritePhase const& Phase = Spr( (int32_t)GetState() );
            renderableSprites.push_back(
                RenderableSprite( &actor, mActionId, &Spr, &Phase/*, color*/) );
        }
    }
}

} // namespace render

