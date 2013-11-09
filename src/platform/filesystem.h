#ifndef INCLUDED_PLATFORM_FILESYSTEM_H
#define INCLUDED_PLATFORM_FILESYSTEM_H

class Filesys : public Singleton<Filesys>
{
	friend class Singleton<Filesys>;
	Filesys();

	typedef boost::ptr_map<int32_t,Package> PackageMap_t;
	PackageMap_t mPackages;
	int32_t mNextPrio;	//todo
public:
	void Mount(std::auto_ptr<Package> Pack);
	AutoFile Open(const boost::filesystem::path& Path);
	typedef Package::PathVect_t PathVect_t;
	void GetFileNames(PathVect_t& Paths, boost::filesystem::path const& Dir=boost::filesystem::path());
};

#endif//INCLUDED_PLATFORM_FILESYSTEM_H
