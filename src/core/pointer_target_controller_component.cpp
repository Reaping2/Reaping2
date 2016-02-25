#include "core/pointer_target_controller_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

PointerTargetControllerComponent::PointerTargetControllerComponent()
    : mPointedTargetGUID(-1)
    , mPointedTargetCounter(0)
    , mNextLaserCounter(0.0)
    , mNextLaserFrequency(0.2)
    , mHeadingModifierCounter(0.0)
    , mHeadingModifierFrequency(0.15)
{
}

void PointerTargetControllerComponent::SetPointedTargetGUID(int32_t pointedTargetId)
{
    mPointedTargetGUID=pointedTargetId;
}

int32_t PointerTargetControllerComponent::GetPointedTargetGUID()const
{
    return mPointedTargetGUID;
}

void PointerTargetControllerComponent::SetPointedTargetCounter(int32_t pointedTargetCounter)
{
    mPointedTargetCounter=pointedTargetCounter;
}

int32_t PointerTargetControllerComponent::GetPointedTargetCounter()const
{
    return mPointedTargetCounter;
}

void PointerTargetControllerComponent::SetNextLaserCounter(double nextLaserCounter)
{
    mNextLaserCounter=nextLaserCounter;
}

double PointerTargetControllerComponent::GetNextLaserCounter()const
{
    return mNextLaserCounter;
}

void PointerTargetControllerComponent::SetNextLaserFrequency(double nextLaserFrequency)
{
    mNextLaserFrequency=nextLaserFrequency;
}

double PointerTargetControllerComponent::GetNextLaserFrequency()const
{
    return mNextLaserFrequency;
}

void PointerTargetControllerComponent::SetHeadingModifierCounter(double headingModifierCounter)
{
    mHeadingModifierCounter=headingModifierCounter;
}

double PointerTargetControllerComponent::GetHeadingModifierCounter()const
{
    return mHeadingModifierCounter;
}

void PointerTargetControllerComponent::SetHeadingModifierFrequency(double headingModifierFrequency)
{
    mHeadingModifierFrequency=headingModifierFrequency;
}

double PointerTargetControllerComponent::GetHeadingModifierFrequency()const
{
    return mHeadingModifierFrequency;
}



void PointerTargetControllerComponentLoader::BindValues()
{
}

PointerTargetControllerComponentLoader::PointerTargetControllerComponentLoader()
{
}

BOOST_CLASS_EXPORT_IMPLEMENT(PointerTargetControllerComponent);
