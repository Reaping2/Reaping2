#include "price.h"
#include "platform/jsonreader.h"


void Price::Load( Json::Value const& setters )
{
    Json::GetInt( setters["dark_matter"], mDarkMatter );
    Json::GetInt( setters["key"], mKey );
    Json::GetInt( setters["gem"], mGem );
}
