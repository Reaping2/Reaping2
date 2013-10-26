#ifndef INCLUDED_PLATFORM_IFILE_H
#define INCLUDED_PLATFORM_IFILE_H

class File
{
public:
	virtual ~File(){}
	virtual bool Read(std::string& Data, size_t Size)=0;
	virtual bool Write(const std::string& Data)=0;
	virtual bool ReadAll(std::string& Data)=0;
	virtual bool IsValid()const=0;
};

#endif//INCLUDED_PLATFORM_IFILE_H
