#include "core/attachable_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

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
REAPING2_CLASS_EXPORT_IMPLEMENT(ctf__AttachableComponent, ctf::AttachableComponent);
