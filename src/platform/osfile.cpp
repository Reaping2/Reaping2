#include "i_platform.h"

OsFile::OsFile(const boost::filesystem::path& Path,std::fstream::open_mode Mode)
: mFile(Path.c_str(),Mode)
{
}

OsFile::~OsFile()
{
	mFile.close();
}

bool OsFile::IsValid() const
{
	return mFile.good();
}

bool OsFile::Read(std::string& Data, size_t Size)
{
	if(!IsValid())return false;
	std::string tmp(Size,'\0');
	mFile.read(&tmp[0],Size);
	if(!IsValid())return false;
	using std::swap;
	swap(Data,tmp);
	return true;
}

bool OsFile::Write(const std::string& Data)
{
	if(!IsValid())return false;
	mFile.write(Data.c_str(),Data.size());
	return IsValid();
}

bool OsFile::ReadAll( std::string& Data )
{
	if(!IsValid())return false;
	size_t P=mFile.tellg();
	mFile.seekg(0,std::ios_base::end);
	size_t L=mFile.tellg();
	mFile.seekg(P,std::ios_base::beg);
	return Read(Data,L-P);
}


