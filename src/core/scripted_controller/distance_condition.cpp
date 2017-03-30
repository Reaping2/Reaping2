#include "distance_condition.h"
#include "core/i_position_component.h"
#include "engine/engine.h"
#include "engine/path_system.h"
namespace scriptedcontroller
{

void DistanceCondition::Update( Actor& actor, double Seconds )
{
}

bool DistanceCondition::IsSatisfied() const
{
    return mGreaterThan<=mDistance && mDistance<=mLessThan;
}

void DistanceCondition::Reset( Actor& actor )
{
    mDistance = std::numeric_limits<double>::max();
}

void DistanceCondition::Load( Json::Value const& setters )
{
    ICondition::Load( setters );
    Json::GetDouble( setters["less"], mLessThan );
    Json::GetDouble( setters["greater"], mGreaterThan );
    Json::GetBool( setters["seek_path"], mSeekPath );
}

} // namespace scriptedcontroller

double core::GetDistance( Actor& actor, Actor& target, bool seekPath/*=true */ )
{
    if (seekPath)
    {
        static auto mPathSystem = engine::Engine::Get().GetSystem<engine::path::PathSystem>();
        return mPathSystem->GetDistance( actor, target );
    }
    else
    {
        auto positionC( actor.Get<IPositionComponent>() );
        auto targetPositionC( target.Get<IPositionComponent>() );
        BOOST_ASSERT( targetPositionC.IsValid() );
        auto const distanceV = glm::vec2( positionC->GetX(), positionC->GetY() )
            - glm::vec2( targetPositionC->GetX(), targetPositionC->GetY() );
        return std::sqrt( glm::dot( distanceV, distanceV ) );
    }
}
