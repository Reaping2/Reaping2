#ifndef INCLUDED_CORE_SCRIPTED_CONTROLLER_I_ACT_H
#define INCLUDED_CORE_SCRIPTED_CONTROLLER_I_ACT_H

#include "core/controller_component.h"
#include "core/property_loader.h"
#include "platform/export.h"
#include <vector>

namespace scriptedcontroller
{

class IAct
{
public:
    IAct( int32_t Id );
    IAct() = default;
    virtual ~IAct() = default;
    virtual void Update( double Seconds );
    // "params" passed
    virtual void Load( Json::Value const& setters );

    virtual void Start();
    virtual void Stop();
    virtual bool IsInterruptible();
    virtual bool IsRunning();
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
protected:
    int32_t mId = -1;
    double mDuration = 0.0;
    double mDurationVariance = 0.0;
    double mDurationCurrent = 0.0;
    bool mInterruptible = false;
};

template<class Archive>
void IAct::serialize( Archive& ar, const unsigned int version )
{
    ar & mId;
    ar & mDuration;
    ar & mDurationVariance;
    ar & mDurationCurrent;
    ar & mInterruptible;
}

class DefaultAct : public IAct
{
public:
    using IAct::IAct;
};

} // namespace scriptedcontroller

#endif//INCLUDED_CORE_SCRIPTED_CONTROLLER_I_ACT_H
