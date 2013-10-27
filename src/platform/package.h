#ifndef INCLUDED_PLATFORM_PACKAGE_H
#define INCLUDED_PLATFORM_PACKAGE_H

class PackageBase
{
protected:
	PackageBase(AutoFile F);
	struct Header {
		uint32_t Magic;
		uint32_t Version;
		uint32_t Checksum;
		uint32_t NumFiles;
		Header():Magic(0x5a474b50),Version(1),Checksum(0),NumFiles(){}
	};
	Header mHeader;
	struct FileDesc {
		uint32_t Offset;
		uint32_t FileSize;
	};
	typedef std::map<boost::filesystem::path,FileDesc> FilesMap;
	FilesMap mFiles;
	AutoFile mFile;
};
class PackagedFile : public File
{

};
class Package : public PackageBase
{
	bool LoadHeader();
public:
	Package(AutoFile Source);
	AutoFile Open(const boost::filesystem::path& Path);
};
class PackageWriter : public PackageBase
{
	bool WriteHeader();
	typedef std::map<boost::filesystem::path,boost::filesystem::path> PathMap;
	PathMap mPaths;
public:
	PackageWriter(AutoFile Target);
	void Add(const boost::filesystem::path& Path,const boost::filesystem::path& PathInArchive);
	bool Save();
};

#endif//INCLUDED_PLATFORM_PACKAGE_H
