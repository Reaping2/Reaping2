#include "matrix_grid.h"
#include "..\i_collision_component.h"
#include "..\opt.h"
#include "..\actor.h"
#include "editor_target_system.h"

namespace map {

MatrixGrid::MatrixGrid(int32_t Id)
    : IGrid(Id)
{

}

    
void MatrixGrid::Update(double DeltaTime)
{

}

void MatrixGrid::SetMousePosition(double x, double y)
{
    IGrid::SetMousePosition(x,y);
    mProcessedPosition=mMousePosition;
    Opt<Actor> cursor(EditorTargetSystem::Get()->GetCursor());
    if (!cursor.IsValid())
    {
        return;
    }
    Opt<ICollisionComponent> collisionC(cursor->Get<ICollisionComponent>());
    if (!collisionC.IsValid())
    {
        return;
    }
    double radius=collisionC->GetRadius();
    mProcessedPosition.x=std::floor(mProcessedPosition.x/(radius*2))*(radius*2)+std::floor(radius);
    mProcessedPosition.y=std::floor(mProcessedPosition.y/(radius*2))*(radius*2)+std::floor(radius);
}


} // namespace map

