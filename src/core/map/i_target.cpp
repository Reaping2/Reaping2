#include "i_target.h"

namespace map {

ITarget::ITarget(int32_t Id)
    : mId(Id)
{

}

int32_t ITarget::GetCursorId() const
{
    return mId;
}

int32_t ITarget::GetActorId() const
{
    return mId;
}


} // namespace map
