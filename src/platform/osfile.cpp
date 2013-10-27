#include "i_platform.h"

OsFile::OsFile(const boost::filesystem::path& Path,std::fstream::open_mode Mode)
: mFile(Path.c_str(),Mode|std::ios_base::binary)
, mPosition(0)
{
	if(IsValid()&&(Mode&std::ios_base::in))
	{
		mFile.seekg(0,std::ios_base::end);
		mSize=mFile.tellg();
		mFile.seekg(0,std::ios_base::beg);
	}
	else
	{
		mSize=0;
	}
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
	mPosition=mFile.tellg();
	if(!IsValid())return false;
	using std::swap;
	swap(Data,tmp);
	return true;
}

bool OsFile::Write(const std::string& Data)
{
	if(!IsValid())return false;
	mSize+=Data.size();
	mFile.write(Data.c_str(),Data.size());
	mPosition=mFile.tellg();
	return IsValid();
}

size_t OsFile::GetSize() const
{
	return mSize;
}

size_t OsFile::GetPosition() const
{
	// tellg() is not const, go figure
	return mPosition;
}

bool OsFile::SetPosition(size_t Pos)
{
	mFile.seekg(Pos,std::ios_base::beg);
	mPosition=Pos;
	return size_t(mFile.tellg())==Pos;
}


