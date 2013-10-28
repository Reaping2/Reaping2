#include "i_platform.h"

bool Compression::Deflate(std::string& Dest,const std::string& Source,int Level)
{
    z_stream Strm;

    /* allocate deflate state */
    Strm.zalloc = Z_NULL;
    Strm.zfree = Z_NULL;
    Strm.opaque = Z_NULL;
    int Ret = deflateInit(&Strm, Level);
    if (Ret != Z_OK)
        return false;
	std::string Tmp;

    Strm.avail_in = (uInt)Source.size();
    Strm.next_in = (Bytef*)(void*)(const_cast<char*>(Source.c_str()));

    /* run deflate() on input until output buffer not full, finish
       compression if all of source has been read in */
    do {
        Strm.avail_out = BUFFER_SIZE;
        Strm.next_out = mOutBuffer;
        Ret = deflate(&Strm, Z_FINISH);    /* no bad return value */
        assert(Ret != Z_STREAM_ERROR);  /* state not clobbered */
        unsigned have = BUFFER_SIZE - Strm.avail_out;
		Tmp.append((char*)(void*)mOutBuffer,have);
    } while (Strm.avail_out == 0);
    assert(Strm.avail_in == 0);     /* all input will be used */
    assert(Ret == Z_STREAM_END);    /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&Strm);
	using std::swap;
	swap(Tmp,Dest);
    return true;
}

Compression::Compression()
{

}

bool Compression::Inflate(std::string& Dest,const std::string& Source)
{
	z_stream Strm;
	/* allocate inflate state */
	Strm.zalloc = Z_NULL;
	Strm.zfree = Z_NULL;
	Strm.opaque = Z_NULL;
	int Ret = inflateInit(&Strm);
	if (Ret != Z_OK)
		return false;

	std::string Tmp;

	Strm.avail_in = (uInt)Source.size();
	Strm.next_in = (Bytef*)(void*)(const_cast<char*>(Source.c_str()));

	/* run inflate() on input until output buffer not full */
	do {
		Strm.avail_out = BUFFER_SIZE;
		Strm.next_out = mOutBuffer;
		Ret = inflate(&Strm, Z_NO_FLUSH);
		assert(Ret != Z_STREAM_ERROR);  /* state not clobbered */
		switch (Ret) {
		case Z_NEED_DICT:
			Ret = Z_DATA_ERROR;     /* and fall through */
		case Z_DATA_ERROR:
		case Z_MEM_ERROR:
			(void)inflateEnd(&Strm);
			return false;
		}
		unsigned have = BUFFER_SIZE - Strm.avail_out;
		Tmp.append((char*)(void*)mOutBuffer,have);
	} while (Strm.avail_out == 0);

	/* clean up and return */
	(void)inflateEnd(&Strm);
	using std::swap;
	swap(Tmp,Dest);
	return Ret == Z_STREAM_END;
}

