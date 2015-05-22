#include "core/accuracy_component.h"

AccuracyComponent::AccuracyComponent()
{
    mAccuracy.mBase.Init(0.0,0.0,10000.0);
    mAccuracy.mFlat.Init(0.0,0.0,1000.0);
    mAccuracy.mPercent.Init(0.0,0.0,100.0);
}

Buffable<int32_t>& AccuracyComponent::GetAccuracy()
{
    return mAccuracy;
}

void AccuracyComponent::SetAccuracyBase(int32_t accuracy)
{
    mAccuracy.mBase.Set(accuracy);
}



void AccuracyComponentLoader::BindValues()
{
    Bind("accuracy",func_int32_t(&AccuracyComponent::SetAccuracyBase));
}

AccuracyComponentLoader::AccuracyComponentLoader()
{
}
