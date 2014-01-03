#include "i_platform.h"

OsFile::OsFile(const boost::filesystem::path& Path,std::ios_base::openmode Mode)
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

bool OsFile::Read(void* Data, size_t Size)
{
	if(!IsValid())return false;
	mFile.read((char*)Data,Size);
	mPosition=mFile.tellg();
	return IsValid();
}

bool OsFile::Write(void const* Src, size_t Size)
{
	if(!IsValid())return false;
	mSize+=Size;
	mFile.write((char const *)Src,Size);
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


