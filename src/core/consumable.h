#ifndef INCLUDED_CORE_CONSUMABLE_H
#define INCLUDED_CORE_CONSUMABLE_H
#include <boost/serialization/serialization.hpp>

class Consumable
{
public:
    Consumable(bool active=false);
    bool GetValue()const;
    void SetActive(bool active);
    void SetConsumed(bool consumed);
    bool IsActive()const;
    bool IsConsumed()const;
protected:
    friend class ComponentFactory;
    bool mActive;
    bool mConsumed;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void Consumable::serialize(Archive& ar, const unsigned int version)
{
    ar & mActive;
}

#endif//INCLUDED_CORE_CONSUMABLE_H
