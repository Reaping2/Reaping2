#ifndef INCLUDED_PLATFORM_OSFILE_H
#define INCLUDED_PLATFORM_OSFILE_H

#include <boost/filesystem.hpp>

class OsFile : public File
{
	std::fstream mFile;
	size_t mSize;
	size_t mPosition;
public:
	OsFile(const boost::filesystem::path& Path, std::ios_base::open_mode OpenMode=std::ios_base::in);
	~OsFile();
	virtual bool Read(std::string& Data, size_t Size);
	virtual bool Write(const std::string& Data);
	virtual bool IsValid()const;
	virtual size_t GetSize()const;
	virtual size_t GetPosition()const;
	virtual bool SetPosition(size_t Pos);
};

#endif//INCLUDED_PLATFORM_OSFILE_H
