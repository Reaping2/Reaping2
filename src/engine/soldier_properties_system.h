#ifndef INCLUDED_ENGINE_SOLDIER_PROPERTIES_SYSTEM_H
#define INCLUDED_ENGINE_SOLDIER_PROPERTIES_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/program_state.h"

namespace engine {

class SoldierPropertiesSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(SoldierPropertiesSystem)
    SoldierPropertiesSystem();
    ~SoldierPropertiesSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
     ModelValue mSoldierModel;
     ModelValue mHealthPressModel;
     ModelValue mAccuracyPressModel;
     ModelValue mSpeedPressModel;

    boost::ptr_vector<ModelValue> mPropertyModels;

    Scene& mScene;
    core::ProgramState& mProgramState;
    void HealthPress( std::string modifier );
    void AccuracyPress( std::string modifier );
    void SpeedPress( std::string modifier );
    bool ModifyPoints( int32_t& currentProperty, std::string modifier );
};

} // namespace engine

#endif//INCLUDED_ENGINE_SOLDIER_PROPERTIES_SYSTEM_H

