#include "id_storage.h"
#include <map>

namespace platform {
namespace detail {

class IdStorageImpl
{
    static const int32_t mReservedIds;
    int32_t mNextId;
    typedef std::map<std::string, int32_t> IdMap_t;
    IdMap_t mIdMap;
public:
    IdStorageImpl();
    int32_t GetId( const std::string& Name );
    bool GetName( int32_t Id, std::string& Name )const;
};

const int32_t IdStorageImpl::mReservedIds = 0x0100;

IdStorageImpl::IdStorageImpl()
    : mNextId( mReservedIds )
{

}

int32_t IdStorageImpl::GetId( const std::string& Name )
{
    int32_t& Id = mIdMap[Name];
    if( !Id )
    {
        Id = ++mNextId;
    }
    return Id;
}

bool IdStorageImpl::GetName( int32_t Id, std::string& Name ) const
{
    for( IdMap_t::const_iterator i = mIdMap.begin(), e = mIdMap.end(); i != e; ++i )
    {
        if( i->second == Id )
        {
            Name = i->first;
            return true;
        }
    }
    return false;
}

} // namespace detail

IdStorage::IdStorage()
{
    mImpl.reset( new detail::IdStorageImpl );
}

int32_t IdStorage::GetId( std::string const& Name )
{
    return mImpl->GetId( Name );
}

bool IdStorage::GetName( int32_t Id, std::string& Name ) const
{
    return mImpl->GetName( Id, Name );
}

IdStorage::~IdStorage()
{
}

} // namespace platform
