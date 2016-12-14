#ifndef INCLUDED_CORE_SCRIPTED_CONTROLLER_STATE_H
#define INCLUDED_CORE_SCRIPTED_CONTROLLER_STATE_H

#include "core/controller_component.h"
#include "core/property_loader.h"
#include "platform/export.h"
#include <vector>
#include "transition.h"
#include <boost/serialization/vector.hpp>
#include "i_act.h"
#include "boost/ptr_container/serialize_ptr_vector.hpp"
#include "core/actor.h"
namespace scriptedcontroller {

struct State
{
    State() = default;
    State( int32_t Id );
    void UpdateTransitions( Actor& actor, double Seconds );
    int32_t GetNextStateIdentifier() const;
    void Update( Actor& actor, double Seconds );
    void Reset( Actor& actor );
    void Load( Json::Value const& setters );
    bool IsStart();
    int32_t GetIdentifier() const;

    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );

private:
    void ShuffleActIndices();
    int32_t mIdentifier = -1;
    bool mIsStart = false;
    typedef std::vector<Transition> Transitions_t;
    Transitions_t mTransitions;
    typedef boost::ptr_vector<IAct> Acts_t;
    Acts_t mActs;
    typedef std::vector<int32_t> WeightedActIndices_t;
    WeightedActIndices_t mActIndices;
    int32_t mCurrActIndex = -1;
    int32_t mNextIdentifier = -1;
};

template<class Archive>
void scriptedcontroller::State::serialize( Archive& ar, const unsigned int version )
{
    ar & mIdentifier;
    ar & mIsStart;
    ar & mTransitions;
    ar & mActs;
    ar & mActIndices;
}

} // namespace scriptedcontroller

#endif//INCLUDED_CORE_SCRIPTED_CONTROLLER_STATE_H
