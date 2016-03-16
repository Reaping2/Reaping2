#include "i_brush.h"
#include "../i_position_component.h"
#include "../i_collision_component.h"
#include "editor_grid_system.h"
#include "editor_target_system.h"
#include "editor_system.h"

namespace map {

IBrush::IBrush( int32_t Id )
    : mId( Id )
    , mScene( Scene::Get() )
{

}

std::vector<int32_t> IBrush::GetActorsToRemove()
{
    std::vector<int32_t> r;
    EditorLayer::Type editorLayer = EditorSystem::Get()->GetEditorLayerType();
    glm::vec2 mousePos = EditorGridSystem::Get()->GetGrid().GetMousePosition();
    int32_t curGUID = EditorTargetSystem::Get()->GetCursor()->GetGUID();
    Opt<IRenderableComponent> cursorRenderableC( EditorTargetSystem::Get()->GetCursor()->Get<IRenderableComponent>() );
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
        Actor& actor = **it;
        Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
        Opt<ICollisionComponent> collisionC = actor.Get<ICollisionComponent>();
        if ( !positionC.IsValid() || !collisionC.IsValid() )
        {
            continue;
        }
        if ( curGUID != actor.GetGUID()
             && std::abs( positionC->GetX() - mousePos.x ) < collisionC->GetRadius()
             && std::abs( positionC->GetY() - mousePos.y ) < collisionC->GetRadius() )
        {
            Opt<IRenderableComponent> renderableC( actor.Get<IRenderableComponent>() );
            if ( editorLayer == EditorLayer::Any
                 || ( editorLayer == EditorLayer::Target
                      && renderableC.IsValid()
                      && cursorRenderableC.IsValid()
                      && renderableC->GetLayer() == cursorRenderableC->GetLayer() ) )
            {
                r.push_back( actor.GetGUID() );
            }
        }
    }
    return r;
}

IBrush::~IBrush()
{

}

} // namespace map

