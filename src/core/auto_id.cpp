#include "i_core.h"
AutoId::AutoId(std::string const& Name)
	: mId(IdStorage::Get().GetId(Name))
{
}
int32_t AutoId::GetId() const
{
	return mId;
}
