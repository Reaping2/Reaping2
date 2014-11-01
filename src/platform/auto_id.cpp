#include "auto_id.h"
#include "id_storage.h"

namespace platform {

AutoId::AutoId( std::string const& Name )
    : mId( IdStorage::Get().GetId( Name ) )
{
}
int32_t AutoId::GetId() const
{
    return mId;
}
AutoId::~AutoId()
{
}

AutoId::operator int32_t() const
{
    return mId;
}

} // namespace platform

