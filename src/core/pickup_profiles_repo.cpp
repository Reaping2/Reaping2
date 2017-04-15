#include "platform/i_platform.h"
#include "pickup_profiles_repo.h"
#include "platform/filesystem_utils.h"
#include <algorithm>
#include "core/map/level_generator/random.h"

using platform::AutoId;

namespace core {


PickupProfilesRepo::PickupProfilesRepo()
    : Repository<PickupProfile>( mDefault )
    , mDefault( -1, Json::Value() )
{
    fs_utils::for_each( "misc/pickup_profiles", ".json", [&]( Json::Value const& desc )
    {
        int32_t Idx = AutoId( desc["name"].asString() );
        mElements.insert( Idx, new PickupProfile( Idx, desc ) );
    } );
}

PickupProfile::PickupProfile( int32_t id, Json::Value const& setters )
    : mId(id)
{
    auto const& items = setters["items"];
    if (!items.isArray())
    {
        return;
    }
    for (auto&& item : items)
    {
        std::string istr;
        if (Json::GetStr( item["name"], istr ))
        {
            int32_t pickupId = AutoId( istr );
            int32_t weight = 1;
            Json::GetInt( item["weight"], weight );
            std::fill_n( std::back_inserter( mWeightedIndexes ), weight, mItems.size() );
            Item weightedItem;
            weightedItem.mPickupId = pickupId;
            mItems.push_back( weightedItem );
        }
    }
}

PickupProfile::Item const& PickupProfile::Roll()
{
    if (mWeightedIndexes.empty()||mItems.empty())
    {
        return mDefaultItem;
    }
    // TODO: I cannot think of a way of getting RandomGenerator's generator instance
    // if using of map::mRand is a solution for this problem, then mRand should be in core or platform. Or in RandomGenerator.
    std::shuffle( mWeightedIndexes.begin(), mWeightedIndexes.end(), map::mRand );
    return mItems[mWeightedIndexes.front()];
}

} // namespace core

