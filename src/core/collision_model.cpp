#include "collision_model.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/i_collision_component.h"

CollisionModel::Object CollisionModel::ObjectFromActor( Actor const& ObjA )
{
    Opt<ICollisionComponent> const objAcollisionC = ObjA.Get<ICollisionComponent>();
    Opt<IPositionComponent> const objApositionC = ObjA.Get<IPositionComponent>();
    if( !objAcollisionC.IsValid()
     || !objApositionC.IsValid() )
    {
        return Object{ glm::vec2(), glm::vec2(), 0 };
    }
    Opt<IMoveComponent> ObjAmoveC = ObjA.Get<IMoveComponent>();
    glm::vec2 speedA;
    if( ObjAmoveC.IsValid() )
    {
        speedA = glm::vec2( ObjAmoveC->GetSpeedX(),
                ObjAmoveC->GetSpeedY() );
    }

    Object ObjOA{ glm::vec2( objApositionC->GetX(), objApositionC->GetY() ),
            speedA,
            objAcollisionC->GetRadius() };
    return ObjOA;
}

bool CollisionModel::AreActorsColliding( Actor const& ObjA, Actor const& ObjB, double Dt ) const
{
    Opt<ICollisionComponent> const objAcollisionC = ObjA.Get<ICollisionComponent>();
    Opt<ICollisionComponent> const objBcollisionC = ObjB.Get<ICollisionComponent>();
    Opt<IPositionComponent> const objApositionC = ObjA.Get<IPositionComponent>();
    Opt<IPositionComponent> const objBpositionC = ObjB.Get<IPositionComponent>();
    if ( !objAcollisionC.IsValid()
      || !objBcollisionC.IsValid()
      || !objApositionC.IsValid()
      || !objBpositionC.IsValid() )
    {
        return false;
    }

    Object ObjOA( ObjectFromActor( ObjA ) ),
           ObjOB( ObjectFromActor( ObjB ) );

    return AreActorsColliding( ObjOA, ObjOB, Dt );
}


