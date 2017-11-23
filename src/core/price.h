#ifndef INCLUDED_CORE_PRICE_H
#define INCLUDED_CORE_PRICE_H

#include <portable_oarchive.hpp>
#include <portable_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include "json/json.h"
#include <boost/serialization/map.hpp>

struct Price
{
    int32_t mDarkMatter = 0;
    using KeyMap_t = std::map<int32_t, int32_t>;
    KeyMap_t mKeys;
    // don't forget method Clear();

    void Load( Json::Value const& setters );
    void Clear();
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void Price::serialize( Archive& ar, const unsigned int version )
{
    ar& mDarkMatter;
    ar& mKeys;
}

#endif//INCLUDED_CORE_PRICE_H
