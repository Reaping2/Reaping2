#ifndef INCLUDED_CORE_PICKUP_DESC_REPO_H
#define INCLUDED_CORE_PICKUP_DESC_REPO_H

#include "platform/repository.h"
#include "platform/singleton.h"
#include "json/json.h"
#include "price.h"
#include "item_type.h"

namespace core {

struct PickupDesc
{
    Price mPrice;
    int32_t mPickupContent = -1;
    ItemType::Type mType = ItemType::Normal;
    PickupDesc( int32_t id, Json::Value const& setters );
};

class PickupDescRepo : public platform::Repository<PickupDesc>, public platform::Singleton<PickupDescRepo>
{
    friend class platform::Singleton<PickupDescRepo>;
    PickupDesc const mDefault;
    PickupDescRepo();
};

} // namespace core

#endif//INCLUDED_CORE_PICKUP_DESC_REPO_H

//command:  "classgenerator.exe" -g "repository" -c "pickup_desc_repo" -t "pickup_desc"
