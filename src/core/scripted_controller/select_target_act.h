#ifndef INCLUDED_CORE_SCRIPTED_CONTROLLER_SELECT_TARGET_ACT_H
#define INCLUDED_CORE_SCRIPTED_CONTROLLER_SELECT_TARGET_ACT_H

#include "i_act.h"
#include "platform/frequency_timer.h"
#include "selection_type.h"
#include "../scene.h"
#include "../i_target_holder_component.h"
#include "../program_state.h"

namespace scriptedcontroller
{

class SelectTargetAct : public IAct
{
public:
    DEFINE_ACT_BASE( SelectTargetAct );
    virtual void Update( Actor& actor, double Seconds );
    virtual void Load( Json::Value const& setters );
    virtual void Start( Actor& actor );
    virtual void Stop( Actor& actor );

    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
private:
    typedef std::vector<SelectionType::Type> Selections_t;
    Selections_t mSelections;
    FrequencyTimer mTimer;
    Scene& mScene = Scene::Get();
    core::ProgramState& mProgramState = core::ProgramState::Get();
    bool SelectCurrentTarget( Actor& actor, ITargetHolderComponent& targetHolderC );
    bool SelectLastHitByTarget( Actor& actor, ITargetHolderComponent& targetHolderC );
    bool SelectClosestTarget( Actor& actor, ITargetHolderComponent& targetHolderC );
    double mLastDamageTime = -1.0;
    int32_t mLastDamageOwnerGUID = -1;
    bool mSeekPath = true;
};

template<class Archive>
void SelectTargetAct::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<IAct>( *this );
}


} // namespace scriptedcontroller

#endif//INCLUDED_CORE_SCRIPTED_CONTROLLER_SELECT_TARGET_ACT_H
