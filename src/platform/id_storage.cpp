#include "i_platform.h"

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

bool IdStorage::GetName( int32_t Id, std::string& Name ) const
{
	for(IdMap_t::const_iterator i=mIdMap.begin(),e=mIdMap.end();i!=e;++i)
	{
		if(i->second==Id)
		{
			Name=i->first;
			return true;
		}
	}
	return false;
}
