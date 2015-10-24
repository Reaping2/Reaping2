#include "core/attachable_component.h"

namespace ctf {

AttachableComponent::AttachableComponent()
    : mAttachedGUID(-1)
{
}

void AttachableComponent::SetAttachedGUID(int32_t attachedGUID)
{
    mAttachedGUID=attachedGUID;
}

int32_t AttachableComponent::GetAttachedGUID()const
{
    return mAttachedGUID;
}



void AttachableComponentLoader::BindValues()
{
}

AttachableComponentLoader::AttachableComponentLoader()
{
}

} // namespace ctf