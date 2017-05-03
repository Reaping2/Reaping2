#include "price.h"
#include "platform/jsonreader.h"
#include "platform/auto_id.h"


void Price::Load( Json::Value const& setters )
{
    Json::GetInt( setters["dark_matter"], mDarkMatter );
    auto const& keysJson = setters["keys"];
    if (keysJson.isArray())
    {
        for (auto&& keyJson : keysJson)
        {
            std::string istr;
            int32_t cnt;
            if (Json::GetStr( keyJson["name"], istr )
                && Json::GetInt( keyJson["count"], cnt ))
            {
                mKeys[platform::AutoId( istr )] += cnt;
            }
        }
    }
}
