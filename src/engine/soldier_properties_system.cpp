#include "platform/i_platform.h"
#include "soldier_properties_system.h"
#include "ui/ui.h"

namespace engine {

SoldierPropertiesSystem::SoldierPropertiesSystem()
    : mScene( Scene::Get() )
    , mProgramState ( core::ProgramState::Get() )
    , mSoldierModel( "soldier", &RootModel::Get() )
    , mHealthPressModel( StringFunc ( this, &SoldierPropertiesSystem::HealthPress ), "health_press", &mSoldierModel ) 
    , mAccuracyPressModel( StringFunc ( this, &SoldierPropertiesSystem::AccuracyPress ), "accuracy_press", &mSoldierModel ) 
    , mSpeedPressModel( StringFunc ( this, &SoldierPropertiesSystem::SpeedPress ), "speed_press", &mSoldierModel ) 
{
}


void SoldierPropertiesSystem::Init()
{
     mPropertyModels.push_back(new ModelValue( mProgramState.mSoldierProperties.mHealth, "health", &mSoldierModel));
     mPropertyModels.push_back(new ModelValue( mProgramState.mSoldierProperties.mAccuracy, "accuracy", &mSoldierModel));
     mPropertyModels.push_back(new ModelValue( mProgramState.mSoldierProperties.mMoveSpeed, "speed", &mSoldierModel));
     mPropertyModels.push_back(new ModelValue( mProgramState.mSoldierProperties.mPoints, "points", &mSoldierModel));
}


void SoldierPropertiesSystem::Update(double DeltaTime)
{

}

void SoldierPropertiesSystem::HealthPress(std::string modifier)
{
    ModifyPoints(mProgramState.mSoldierProperties.mHealth,modifier);
}

void SoldierPropertiesSystem::AccuracyPress(std::string modifier)
{
    ModifyPoints(mProgramState.mSoldierProperties.mAccuracy,modifier);
}

void SoldierPropertiesSystem::SpeedPress(std::string modifier)
{
    ModifyPoints(mProgramState.mSoldierProperties.mMoveSpeed,modifier);
}

bool SoldierPropertiesSystem::ModifyPoints( int32_t& currentProperty, std::string modifier )
{
    bool addition=modifier=="+";

    if(addition&&currentProperty<mProgramState.mSoldierProperties.mPoints)
    {
        ++currentProperty;
        mProgramState.mSoldierProperties.mPoints-=currentProperty;
        return true;
    }
    if (!addition&&currentProperty>0)
    {
        mProgramState.mSoldierProperties.mPoints+=currentProperty;
        --currentProperty;
        return true;
    }
    return false;
}

SoldierPropertiesSystem::~SoldierPropertiesSystem()
{
    mPropertyModels.clear();
}


} // namespace engine

