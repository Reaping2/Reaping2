#ifndef INCLUDED_CORE_PICKUP_PROFILES_REPO_H
#define INCLUDED_CORE_PICKUP_PROFILES_REPO_H

#include "platform/repository.h"
#include "platform/singleton.h"
#include <vector>
#include "json/json.h"

namespace core {

struct PickupProfile
{
    struct Item
    {
        int32_t mPickupId = -1;
    };
    PickupProfile( int32_t id, Json::Value const& setters );
    PickupProfile::Item const& Roll();
private:
    int32_t mId = -1;
    std::vector<Item> mItems;
    std::vector<int32_t> mWeightedIndexes;
    Item mDefaultItem;
};


class PickupProfilesRepo : public platform::Repository<PickupProfile>, public platform::Singleton<PickupProfilesRepo>
{
    friend class platform::Singleton<PickupProfilesRepo>;
    PickupProfile const mDefault;
    PickupProfilesRepo();
};

} // namespace core

#endif//INCLUDED_CORE_PICKUP_PROFILES_REPO_H

//command:  "classgenerator.exe" -g "repository" -c "pickup_profiles_repo" -t "pickup_profile"
