#ifndef INCLUDED_PLATFORM_COMPRESSION_H
#define INCLUDED_PLATFORM_COMPRESSION_H

class Compression : public Singleton<Compression>
{
	friend class Singleton<Compression>;
	Compression();
	enum {
		BUFFER_SIZE=128*1024,
	};
	unsigned char mOutBuffer[BUFFER_SIZE];
public:
	bool Deflate(std::string& Dest,const std::string& Source,int Level=Z_DEFAULT_COMPRESSION);
	bool Inflate(std::string& Dest,const std::string& Source);
};

#endif//INCLUDED_PLATFORM_COMPRESSION_H
