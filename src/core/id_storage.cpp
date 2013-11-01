#include "i_core.h"

const int32_t IdStorage::mReservedIds=0x0100;

IdStorage::IdStorage()
: mNextId(mReservedIds)
{

}

int32_t IdStorage::GetId( const std::string& Name )
{
	int32_t& Id=mIdMap[Name];
	if(!Id)Id=++mNextId;
	return Id;
}

