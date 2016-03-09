#ifndef INCLUDED_CORE_CONSUMABLE_H
#define INCLUDED_CORE_CONSUMABLE_H
#include <boost/serialization/serialization.hpp>

class Trigger
{
public:
    Trigger();
    void Activate();
    void Deactivate();
    void Handled();
    bool GetValue()const;
    bool IsActive()const;
protected:
    friend class ComponentFactory;
    bool mActive;
    bool mHandled;
    bool mTriggered;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void Trigger::serialize( Archive& ar, const unsigned int version )
{
    ar & mActive;
}
#endif//INCLUDED_CORE_CONSUMABLE_H
