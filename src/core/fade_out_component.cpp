#include "platform/i_platform.h"
#include "core/fade_out_component.h"
#include "core/scene.h"

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
