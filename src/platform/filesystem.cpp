#include "i_platform.h"

void Filesys::Mount( std::auto_ptr<Package> Pack )
{
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
	AutoFile Ret;
	for(PackageMap_t::iterator i=mPackages.begin(),e=mPackages.end();i!=e&&!Ret.get();++i)
		Ret=i->second->Open(Path);
	return Ret;
}

void Filesys::GetFileNames( PathVect_t& Paths, boost::filesystem::path const& Dir/*=boost::filesystem::path()*/ )
{
	for(PackageMap_t::iterator i=mPackages.begin(),e=mPackages.end();i!=e;++i)
		i->second->GetFileNames(Paths,Dir);
}

