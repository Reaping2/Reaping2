#include "platform/i_platform.h"
#include "core/fade_out_component.h"
#include "core/scene.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

using platform::AutoId;

void FadeOutComponent::Update(double Seconds)
{
}

FadeOutComponent::FadeOutComponent()
    : mSecsToEnd(0.0)
{
}

double FadeOutComponent::GetSecsToEnd()
{
    return mSecsToEnd;
}

void FadeOutComponent::SetSecsToEnd(double secsToEnd)
{
    mSecsToEnd=secsToEnd;
}

FadeOutComponentLoader::FadeOutComponentLoader()
{
}

void FadeOutComponentLoader::BindValues()
{
    Bind("secs_to_end",func_double(&FadeOutComponent::SetSecsToEnd));
}

REAPING2_CLASS_EXPORT_IMPLEMENT(FadeOutComponent, FadeOutComponent);
