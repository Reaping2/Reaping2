#ifndef INCLUDED_PLATFORM_OSFILE_H
#define INCLUDED_PLATFORM_OSFILE_H

#include <boost/filesystem.hpp>

class OsFile : public File
{
	std::fstream mFile;
public:
	OsFile(const boost::filesystem::path& Path, std::ios_base::open_mode OpenMode=std::ios_base::in);
	~OsFile();
	virtual bool Read(std::string& Data, size_t Size);
	virtual bool Write(const std::string& Data);
	virtual bool ReadAll(std::string& Data);
	virtual bool IsValid()const;
};

#endif//INCLUDED_PLATFORM_OSFILE_H
