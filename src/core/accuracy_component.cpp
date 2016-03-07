#include "core/accuracy_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

AccuracyComponent::AccuracyComponent()
{
    mAccuracy.mBase.Init( 0.0, 0.0, 10000.0 );
    mAccuracy.mFlat.Init( 0.0, 0.0, 1000.0 );
    mAccuracy.mPercent.Init( 0.0, 0.0, 100.0 );
}

Buffable<int32_t>& AccuracyComponent::GetAccuracy()
{
    return mAccuracy;
}

void AccuracyComponent::SetAccuracyBase( int32_t accuracy )
{
    mAccuracy.mBase.Set( accuracy );
}



void AccuracyComponentLoader::BindValues()
{
    Bind( "accuracy", func_int32_t( &AccuracyComponent::SetAccuracyBase ) );
}

AccuracyComponentLoader::AccuracyComponentLoader()
{
}

REAPING2_CLASS_EXPORT_IMPLEMENT( AccuracyComponent, AccuracyComponent );
