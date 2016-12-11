#ifndef INCLUDED_CORE_SCRIPTED_CONTROLLER_I_CONDITION_H
#define INCLUDED_CORE_SCRIPTED_CONTROLLER_I_CONDITION_H

#include "core/controller_component.h"
#include "core/property_loader.h"
#include "platform/export.h"
#include <vector>

namespace scriptedcontroller
{

class ICondition
{
public:
    ICondition( int32_t Id );
    ICondition() = default;
    virtual ~ICondition() = default;
    virtual void Update( double Seconds );
    virtual bool IsSatisfied();
    virtual void Load( Json::Value const& setters );

    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
protected:
    int32_t mId = -1;
};

template<class Archive>
void ICondition::serialize( Archive& ar, const unsigned int version )
{
    ar & mId;
}

class DefaultCondition : public ICondition
{
public:
    using ICondition::ICondition;
};

} // namespace scriptedcontroller

#endif//INCLUDED_CORE_SCRIPTED_CONTROLLER_I_CONDITION_H
