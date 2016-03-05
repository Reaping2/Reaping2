#include "id_storage.h"
#include "init.h"
#include "filesystem.h"
#include "ifile.h"
#include <map>

namespace platform {
namespace {
void InitAutoIDs()
{
    Filesys& fs( Filesys::Get() );
    std::auto_ptr<File> f( fs.Open( "autoids" ) );
    if( NULL == f.get() )
    {
        return;
    }
    std::string data;
    f->ReadAll( data );
    IdStorage& ids( IdStorage::Get() );
    size_t linestart = 0, lineend = std::string::npos;
    do {
        lineend = data.find( '\n', linestart );
        auto const& l = data.substr( linestart, ( lineend == std::string::npos ? lineend : ( lineend - linestart ) ) );
        ids.GetId( l );
        linestart = lineend + 1;
    } while( lineend != std::string::npos );
}
REGISTER_INIT_PRIO( aaa, InitAutoIDs, &InitAutoIDs )
}
namespace detail {

class IdStorageImpl
{
    static const int32_t mReservedIds;
    int32_t mNextId;
    typedef std::map<std::string, int32_t> IdMap_t;
    IdMap_t mIdMap;
    bool mInited;
public:
    IdStorageImpl();
    int32_t GetId( const std::string& Name );
    bool GetName( int32_t Id, std::string& Name )const;
    void Init();
};

const int32_t IdStorageImpl::mReservedIds = 0x0100;

IdStorageImpl::IdStorageImpl()
    : mNextId( mReservedIds )
    , mInited( false )
{

}

void IdStorageImpl::Init()
{
    mInited = true;
}

int32_t IdStorageImpl::GetId( const std::string& Name )
{
    BOOST_ASSERT( mInited );
    int32_t& Id = mIdMap[Name];
    if( !Id )
    {
        Id = ++mNextId;
    }
    return Id;
}

bool IdStorageImpl::GetName( int32_t Id, std::string& Name ) const
{
    BOOST_ASSERT( mInited );
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

void IdStorage::Init()
{
    mImpl->Init();
}

IdStorage::~IdStorage()
{
}

} // namespace platform
