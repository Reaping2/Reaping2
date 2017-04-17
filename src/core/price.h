#ifndef INCLUDED_CORE_PRICE_H
#define INCLUDED_CORE_PRICE_H

#include <portable_oarchive.hpp>
#include <portable_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include "json/json.h"

struct Price
{
    int32_t mDarkMatter = 0;
    int32_t mKey = 0;
    int32_t mGem = 0;

    void Load( Json::Value const& setters );

    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void Price::serialize( Archive& ar, const unsigned int version )
{
    ar& mDarkMatter;
    ar& mKey;
    ar& mGem;
}

#endif//INCLUDED_CORE_PRICE_H
