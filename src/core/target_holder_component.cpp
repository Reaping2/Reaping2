#include "core/target_holder_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

TargetHolderComponent::TargetHolderComponent()
    : mTargetGUID( -1 )
{
}

void TargetHolderComponent::SetTargetGUID( int32_t targetId )
{
    mTargetGUID = targetId;
}

int32_t TargetHolderComponent::GetTargetGUID()const
{
    return mTargetGUID;
}



void TargetHolderComponentLoader::BindValues()
{
}

TargetHolderComponentLoader::TargetHolderComponentLoader()
{
}

REAPING2_CLASS_EXPORT_IMPLEMENT( TargetHolderComponent, TargetHolderComponent );
