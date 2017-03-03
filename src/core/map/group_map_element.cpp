#include "core/map/group_map_element.h"

namespace map {

GroupMapElement::GroupMapElement( int32_t Id )
    : MapElement( Id )
    , mTargets()
{
}

void GroupMapElement::Load( Json::Value const& setters )
{
    MapElement::Load( setters );
    mTargets.clear();
    auto const& targets = setters["targets"];
    if (targets.isArray())
    {
        for (auto& target : targets)
        {
            if (target.isString())
            {
                mTargets.insert( AutoId( target.asString() ) );
            }
        }
    }
}


void GroupMapElement::Save( Json::Value& Element )
{
    MapElement::Save( Element );
    IdStorage& idStorage = IdStorage::Get();
    Json::Value TargetArr( Json::arrayValue );
    for (auto& target : mTargets)
    {
        std::string targetName;
        if (idStorage.GetName( target, targetName ))
        {
            Json::Value jName = Json::Value( targetName );
            TargetArr.append( jName );
        }
    }
    Element["targets"] = TargetArr;
}

void GroupMapElement::SetTargets( Targets_t targets )
{
    mTargets = targets;
}

GroupMapElement::Targets_t const& GroupMapElement::GetTargets() const
{
    return mTargets;
}


} // namespace map
