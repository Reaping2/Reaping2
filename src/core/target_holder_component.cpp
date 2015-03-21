#include "core/target_holder_component.h"

TargetHolderComponent::TargetHolderComponent()
    : mTarget(NULL)
{
}

void TargetHolderComponent::SetTarget(Actor* target)
{
    mTarget=target;
}

Actor* TargetHolderComponent::GetTarget()const
{
    return mTarget;
}



void TargetHolderComponentLoader::BindValues()
{
}

TargetHolderComponentLoader::TargetHolderComponentLoader()
{
}
