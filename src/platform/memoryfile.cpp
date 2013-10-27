#include "i_platform.h"

MemoryFile::MemoryFile(size_t PreallocatedBuffers/*=0*/)
: mActBufferId(0)
, mSize(0)
{
	if(PreallocatedBuffers)
		mBuffers.resize(PreallocatedBuffers);
}

bool MemoryFile::Read(std::string& Data, size_t Size)
{
	if(Size>GetSize()-GetPosition())return false;
	std::string Tmp(Size,'\0');
	char* p(&Tmp[0]);
	while(Size)
	{
		const size_t R=mBuffers[mActBufferId].read(p,Size);
		Size-=R;
		p+=R;
		if(Size)
			mActBufferId++;
		if(mActBufferId>=mBuffers.size())
			return false;
	}
	using std::swap;
	swap(Tmp,Data);
	return true;
}

size_t MemoryFile::GetSize() const
{
	return mSize;
}

bool MemoryFile::Write(const std::string& Data)
{
	size_t Size(Data.size());
	mSize+=Size;
	char const*p(&Data[0]);
	while(Size)
	{
		if(mActBufferId>=mBuffers.size())
			mBuffers.push_back(Buffer_t());
		const size_t W=mBuffers[mActBufferId].write(p,Size);
		Size-=W;
		p+=W;
		if(Size)
			mActBufferId++;
	}
	return true;
}

bool MemoryFile::IsValid() const
{
	return true;
}

bool MemoryFile::CopyTo( File& Other )
{
	for(Buffers::const_iterator i=mBuffers.begin(),e=mBuffers.end();i!=e;++i)
	{
		if(!i->size)break;
		std::string Data(i->data,i->size);
		if(!Other.Write(Data))return false;
	}
	return true;
}

bool MemoryFile::CopyFrom( File& Other )
{
	size_t Size=Other.GetSize();
	mActBufferId=0;
	mSize=Size;
	const size_t NumBuffers=Size/ONE_BUFFER_SIZE+(Size%ONE_BUFFER_SIZE)?1:0;
	mBuffers.clear();
	mBuffers.resize(NumBuffers);
	size_t Idx=0;
	while(Size)
	{
		size_t ToRead=std::min<size_t>(Size,ONE_BUFFER_SIZE);
		std::string Buffer;
		if(!Other.Read(Buffer,ToRead))
		{
			mBuffers.clear();
			mSize=0;
			return false;
		}
		memcpy(mBuffers[Idx++].data,Buffer.data(),ToRead);
		Size-=ToRead;
	}
	return true;
}

bool MemoryFile::Rewind()
{
	mActBufferId=0;
	for(Buffers::iterator i=mBuffers.begin(),e=mBuffers.end();i!=e;++i)
		i->pos=0;
	return true;
}

size_t MemoryFile::GetPosition() const
{
	return mActBufferId*ONE_BUFFER_SIZE+((mActBufferId<mBuffers.size())?mBuffers[mActBufferId].pos:0);
}

bool MemoryFile::SetPosition( size_t Pos )
{
	assert(false);
	// ha kell, implementald, pillanatnyilag nem latom ertelmet
	return false;
}

size_t MemoryFile::Buffer_t::write( void const * d, size_t s )
{
	const size_t Avail=ONE_BUFFER_SIZE-pos;
	const size_t ToWrite=std::min<size_t>(Avail,s);
	memcpy(data+pos,d,ToWrite);
	pos+=ToWrite;
	if(pos>size)
		size=pos;
	return ToWrite;
}

size_t MemoryFile::Buffer_t::read( void * d, size_t s )
{
	const size_t ToRead=std::min<size_t>(size-pos,s);
	memcpy(d,data+pos,ToRead);
	pos+=ToRead;
	return ToRead;
}
