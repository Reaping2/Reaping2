#include "render/cell_action_renderer.h"
#include "renderable_sprite.h"
#include "core/i_cell_component.h"
#include "core/map/level_generator/entrance_type.h"

namespace render {

CellActionRenderer::CellActionRenderer(int32_t Id)
    : ActionRenderer(Id, AutoId("idle"))
{
}

void CellActionRenderer::FillRenderableSprites(const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites)
{
    SpriteCollection const& Sprites=mRenderableRepo(actor.GetId());
    auto CellC( actor.Get<ICellComponent>() );
    if (!CellC.IsValid())
    {
        return;
    }
    if (mSpr != nullptr)
    {
        SpritePhase const& Phase = (*mSpr)( (int32_t)GetState() );

        glm::vec4 col = CellC->GetRoomDesc()->IsFilled(CellC->GetX(),CellC->GetY()) 
            ? glm::vec4( 1, 1, 0, 1 ) : glm::vec4( 1, 1, 1, 1 );
        renderableSprites.push_back(
            RenderableSprite( &actor, &renderableC, mActionId, mSpr, &Phase, col ) );

    }
    static int32_t entranceId = AutoId( "entrance" );
    auto& entranceSpr = Sprites( entranceId );
    if (entranceSpr.IsValid())
    {
        SpritePhase const& Phase = entranceSpr( (int32_t)GetState() );
        auto const& cell = CellC->GetRoomDesc()->GetCell( CellC->GetX(), CellC->GetY() );
        int32_t cellSize = CellC->GetRoomDesc()->GetCellSize()/2;
        glm::vec4 col( 1, 1, 1, 0.3 );
        int32_t relPos = cellSize * 0.82;
        if (cell.HasEntrance( map::EntranceType::Top ))
        {
            auto rendSpr = RenderableSprite( &actor, &renderableC, entranceId, &entranceSpr, &Phase, col );
            rendSpr.RelativePosition = glm::vec2( 0, relPos );
            renderableSprites.push_back( rendSpr );
        }
        if (cell.HasEntrance( map::EntranceType::Right ))
        {
            auto rendSpr = RenderableSprite( &actor, &renderableC, entranceId, &entranceSpr, &Phase, col );
            rendSpr.RelativePosition = glm::vec2( relPos, 0 );
            renderableSprites.push_back( rendSpr );
        }
        if (cell.HasEntrance( map::EntranceType::Bottom ))
        {
            auto rendSpr = RenderableSprite( &actor, &renderableC, entranceId, &entranceSpr, &Phase, col );
            rendSpr.RelativePosition = glm::vec2( 0, -relPos );
            renderableSprites.push_back( rendSpr );
        }
        if (cell.HasEntrance( map::EntranceType::Left ))
        {
            auto rendSpr = RenderableSprite( &actor, &renderableC, entranceId, &entranceSpr, &Phase, col );
            rendSpr.RelativePosition = glm::vec2( -relPos, 0 );
            renderableSprites.push_back( rendSpr );
        }
    }
}

} // namespace render

