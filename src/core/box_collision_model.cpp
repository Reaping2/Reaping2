#include "platform/i_platform.h"
#include "core/box_collision_model.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/i_collision_component.h"

bool BoxCollisionModel::AreActorsColliding( Actor const& ObjA, Actor const& ObjB, double Dt )const
{
    Opt<ICollisionComponent> const objAcollisionC = ObjA.Get<ICollisionComponent>();
    Opt<ICollisionComponent> const objBcollisionC = ObjB.Get<ICollisionComponent>();
    if ( !objAcollisionC.IsValid() || !objBcollisionC.IsValid() )
    {
        //TODO: this one should not happen - later
        //they do not collide, is some of them can't thats for sure
        return false;
    }
    return AreActorsColliding(ObjA, ObjB, objAcollisionC->GetRadius(), objBcollisionC->GetRadius(), Dt);

}

bool BoxCollisionModel::AreActorsColliding(Actor const &ObjA, Actor const &ObjB, double radiusA, double radiusB, double Dt)
{
    // A: (0,0)
    //TODO: this may change, or will be simplified, because this is kind of ugly, to gat these two positions. Time will tell
    Opt<IPositionComponent> const objApositionC = ObjA.Get<IPositionComponent>();
    Opt<IPositionComponent> const objBpositionC = ObjB.Get<IPositionComponent>();
    glm::vec2 B( objBpositionC->GetX() - objApositionC->GetX(), objBpositionC->GetY() - objApositionC->GetY() );
    // BSize: (0,0)
    glm::vec2 ASize( radiusA + radiusB, radiusA + radiusB );
    // on point check
    static const float Epsilon = std::numeric_limits<float>::epsilon() * 100;
    if( std::abs( B.x ) + Epsilon < ASize.x && std::abs( B.y ) + Epsilon < ASize.y )
    {
        return true;
    }
    Opt<IMoveComponent> ObjAmoveC = ObjA.Get<IMoveComponent>();
    Opt<IMoveComponent> ObjBmoveC = ObjB.Get<IMoveComponent>();
    float const ObjASpdX = ObjAmoveC.IsValid()?ObjAmoveC->GetSpeedX():0.0f;
    float const ObjASpdY = ObjAmoveC.IsValid()?ObjAmoveC->GetSpeedY():0.0f;
    float const ObjBSpdX = ObjBmoveC.IsValid()?ObjBmoveC->GetSpeedX():0.0f;
    float const ObjBSpdY = ObjBmoveC.IsValid()?ObjBmoveC->GetSpeedY():0.0f;

    glm::vec2 Spd( ObjBSpdX - ObjASpdX, ObjBSpdY - ObjASpdY );
    glm::vec2 T1minusB = ASize - B;
    glm::vec2 T2minusB = -ASize - B;
    glm::vec2 MinTimes( std::numeric_limits<float>::max() );
    glm::vec2 MaxTimes( -std::numeric_limits<float>::max() );
    static const size_t Dim = 2;
    for( size_t i = 0; i < Dim; ++i )
    {
        if( std::abs( Spd[i] ) <= Epsilon )
        {
            // one dim speed is zero
            if( std::abs( B[i] ) > ASize[i] )
            {
                continue;
            }
            MinTimes[i] = 0;
            MaxTimes[i] = ( float )Dt;
            continue;
        }
        float d1 = T1minusB[i] / Spd[i];
        float d2 = T2minusB[i] / Spd[i];
        float mi = std::min<float>( d1, d2 );
        float ma = std::max<float>( d1, d2 );
        if( ma <= Epsilon )
        {
            // if maxtime is 0/negative, then we're already past the other object, no collision (handle edge cases)
            ma = -1;
            mi = -1;
        }
        MinTimes[i] = mi;
        MaxTimes[i] = ma;
    }
    double MinTime = glm::compMax( MinTimes );
    double MaxTime = glm::compMin( MaxTimes );

    bool const SweepResult = MinTime >= 0 && MinTime <= MaxTime && MinTime <= Dt;
    return SweepResult;
}
