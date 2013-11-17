#include "i_platform.h"

void Filesys::Mount( std::auto_ptr<Package> Pack )
{
	boost::mutex::scoped_lock Lock(mOpenMutex);
	//todo: priority?
	++mNextPrio;
	mPackages.insert(mNextPrio,Pack.release());
}

Filesys::Filesys()
: mNextPrio(0)
{

}

AutoFile Filesys::Open( const boost::filesystem::path& Path )
{
	boost::mutex::scoped_lock Lock(mOpenMutex);
	AutoFile Ret;
	for(PackageMap_t::iterator i=mPackages.begin(),e=mPackages.end();i!=e&&!Ret.get();++i)
		Ret=i->second->Open(Path);
	return Ret;
}

void Filesys::GetFileNames( PathVect_t& Paths, boost::filesystem::path const& Dir/*=boost::filesystem::path()*/ )
{
	boost::mutex::scoped_lock Lock(mOpenMutex);
	for(PackageMap_t::iterator i=mPackages.begin(),e=mPackages.end();i!=e;++i)
		i->second->GetFileNames(Paths,Dir);
}

