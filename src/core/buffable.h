#ifndef INCLUDED_CORE_BUFFABLE_H
#define INCLUDED_CORE_BUFFABLE_H

#include <boost/serialization/serialization.hpp>
#include "platform/export.h"
#include "platform/jsonreader.h"

template<typename T>
class Limited
{
public:
    T GetMax() const;
    Limited<T>& SetMax( T max );
    T GetMin() const;
    Limited<T>& SetMin( T min );
    T Get() const;
    Limited<T>& Set( T val );
    Limited();
    Limited<T>& Init( T val, T min, T max );
    void Load( Json::Value const& desc );
private:
    T mMax;
    T mMin;
    T mVal;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version )
    {
        ar& mMax;
        ar& mMin;
        ar& mVal;
    }
};

template<typename T>
void Limited<T>::Load( Json::Value const& desc )
{
    Json::Get( desc["max"], mMax );
    Json::Get( desc["min"], mMin );
    Json::Get( desc["val"], mVal );
}

template<typename T>
Limited<T>& Limited<T>::Init( T val, T min, T max )
{
    return SetMin( min ).SetMax( max ).Set( val );
}

template<typename T>
Limited<T>::Limited()
    : mMax()
    , mMin()
    , mVal()
{
}

template<typename T>
Limited<T>& Limited<T>::Set( T val )
{
    mVal = std::max( mMin, std::min( mMax, val ) );
    return *this;
}

template<typename T>
T Limited<T>::Get() const
{
    return mVal;
}

template<typename T>
Limited<T>& Limited<T>::SetMin( T min )
{
    mMin = min;
    return *this;
}

template<typename T>
T Limited<T>::GetMin() const
{
    return mMin;
}

template<typename T>
Limited<T>& Limited<T>::SetMax( T max )
{
    mMax = max;
    return *this;
}

template<typename T>
T Limited<T>::GetMax() const
{
    return mMax;
}

template<typename T>
class Buffable
{
public:
    T Get() const;
    Limited<T> mBase;
    Limited<double> mPercent;
    Limited<T> mFlat;
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version )
    {
        ar& mBase;
        ar& mPercent;
        ar& mFlat;
    }
};

template<typename T>
T Buffable<T>::Get() const
{
    return mBase.Get()
           + ( T )( mBase.Get() * ( mPercent.Get() / 100.0 ) )
           + mFlat.Get();
}

#endif//INCLUDED_CORE_BUFFABLE_H
