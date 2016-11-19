#include "id_storage.h"
#include "init.h"
#include "filesystem.h"
#include "ifile.h"
#include <map>

namespace platform {
namespace {
void ReadAutoIDsFromFile(const std::string& fname )
{
    Filesys& fs( Filesys::Get() );
    std::auto_ptr<File> f( fs.Open( fname ) );
    if( NULL == f.get() )
    {
        return;
    }
    std::string data;
    f->ReadAll( data );
    std::istringstream istrm( data );
    IdStorage& ids( IdStorage::Get() );
    while( istrm.good() )
    {
        std::string line;
        std::getline( istrm, line );
        ids.GetId( line );
    }
}

void InitAutoIDs()
{
    ReadAutoIDsFromFile("autoids.content");
    ReadAutoIDsFromFile("autoids.src");
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
