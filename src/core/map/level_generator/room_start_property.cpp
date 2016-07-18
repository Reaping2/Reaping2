#include "core/map/level_generator/room_start_property.h"


namespace map {

RoomStartProperty::RoomStartProperty( int32_t Id )
    : IProperty( Id )
    , mTargets()
{
}

void RoomStartProperty::Load( Json::Value& setters )
{
    IProperty::Load( setters );
    mTargets.clear();
    auto const& targets = setters["targets"];
    if (targets.isArray())
    {
        for (auto& target : targets)
        {
            mTargets.push_back( AutoId( target.asString() ) );
        }
    }
}


void RoomStartProperty::SetTargets( Targets_t blockedTargets )
{
    mTargets = blockedTargets;
}

RoomStartProperty::Targets_t const& RoomStartProperty::GetTargets() const
{
    return mTargets;
}



} // namespace map
