#include "platform/i_platform.h"
#include "activity_traits.h"
#include "player_controller_component.h"
#include "i_collision_component.h"
#include "i_activity_component.h"
#include "platform/settings.h"

namespace core {

ActivityTraits::ActorScaleFunc ActivityTraits::mScaleFunc;
ActivityTraits::ActiveSizeFunc ActivityTraits::mActiveSize;

bool ActivityTraits::AreCorrelated( int32_t CC1, int32_t CC2 )
{
    return CC1 == Active || CC2 == Active;
}

int32_t ActivityTraits::GetTrait( Actor const& actor )
{
    Opt<IActivityComponent> activityC = actor.Get<IActivityComponent>();
    return activityC.IsValid() ? Active : Passive;
}

double ActivityTraits::GetRadius( Actor const& actor )
{
    Opt<PlayerControllerComponent> controllerC = actor.Get<IControllerComponent>();
    if( controllerC.IsValid() )
    {
        static const auto activesize = Settings::Get().GetDouble( "activity.default_active_size", 2000.0 );
        return mActiveSize ? mActiveSize() : activesize;
    }
    Opt<IActivityComponent> activityC = actor.Get<IActivityComponent>();
    double activityRadius = activityC.IsValid() ? activityC->GetRadius() : 0.0;
    Opt<ICollisionComponent> collC = actor.Get<ICollisionComponent>();
    if( !collC.IsValid() )
    {
        return activityRadius;
    }
    float scale = mScaleFunc ? mScaleFunc( actor.GetId() ) : 1.0f;
    return std::max( activityRadius, collC->GetRadius() * scale );
}

void ActivityTraits::SetActorScaleFunc( ActorScaleFunc const& f )
{
    mScaleFunc = f;
}

void ActivityTraits::SetActiveSizeFunc( ActiveSizeFunc const& f )
{
    mActiveSize = f;
}

}
