#ifndef INCLUDED_CORE_SCRIPTED_CONTROLLER_FREELANCE_ACT_H
#define INCLUDED_CORE_SCRIPTED_CONTROLLER_FREELANCE_ACT_H

#include "core/controller_component.h"
#include "core/property_loader.h"
#include "platform/export.h"
#include <vector>
#include "core/actor.h"
#include "i_act.h"

namespace scriptedcontroller
{

class FreelanceAct : public IAct
{
public:
    DEFINE_ACT_BASE( FreelanceAct );
    virtual void Update( Actor& actor, double Seconds );
    virtual void Load( Json::Value const& setters );
    virtual void Start( Actor& actor );
    virtual void Stop( Actor& actor );

    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );

};

template<class Archive>
void FreelanceAct::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<IAct>( *this );
}


} // namespace scriptedcontroller

#endif//INCLUDED_CORE_SCRIPTED_CONTROLLER_FREELANCE_ACT_H
