#include "i_platform.h"

bool Package::LoadHeader()
{
	File& f=*mFile;
	std::string Buffer;
	if(!f.Read(Buffer,sizeof(mHeader.Magic))||!conv::deserialize(Buffer,mHeader.Magic))return false;
	if(!f.Read(Buffer,sizeof(mHeader.Version))||!conv::deserialize(Buffer,mHeader.Version))return false;
	if(!f.Read(Buffer,sizeof(mHeader.Checksum))||!conv::deserialize(Buffer,mHeader.Checksum))return false;
	if(!f.Read(Buffer,sizeof(mHeader.NumFiles))||!conv::deserialize(Buffer,mHeader.NumFiles))return false;
	for(size_t i=0;i<mHeader.NumFiles;++i)
	{
		uint32_t FileNameSize=0;
		if(!f.Read(Buffer,sizeof(FileNameSize))||!conv::deserialize(Buffer,FileNameSize))return false;
		if(!f.Read(Buffer,FileNameSize))return false;
		FileDesc& FD=mFiles[boost::filesystem::path(Buffer)];
		if(!f.Read(Buffer,sizeof(FD.Offset))||!conv::deserialize(Buffer,FD.Offset))return false;
		if(!f.Read(Buffer,sizeof(FD.FileSize))||!conv::deserialize(Buffer,FD.FileSize))return false;
	}
	const size_t BaseOffset=f.GetPosition();
	for(FilesMap::iterator i=mFiles.begin(),e=mFiles.end();i!=e;++i)
		i->second.Offset+=BaseOffset;
	return true;
}

Package::Package( AutoFile Source )
: PackageBase(Source)
{
	if(!mFile.get()||!mFile->IsValid())return;
	LoadHeader();
}

AutoFile Package::Open(const boost::filesystem::path& Path)
{
	FilesMap::const_iterator it=mFiles.find(Path);
	if(it==mFiles.end())return AutoFile();

	// ez nem igy lesz kesobb, de egyelore ezzel mar lehet dolgozni
	if(!mFile->SetPosition(it->second.Offset))return AutoFile();
	std::string Buffer;
	if(!mFile->Read(Buffer,it->second.FileSize))return AutoFile();
	MemoryFile*M=new MemoryFile;
	AutoFile F(M);
	if(!M->Write(Buffer))return AutoFile();
	M->Rewind();
	return F;
}

bool PackageWriter::WriteHeader()
{
	File& f=*mFile;
	std::string Buffer;
	mHeader.NumFiles=mFiles.size();
	if(!f.Write(conv::serialize(mHeader.Magic)))return false;
	if(!f.Write(conv::serialize(mHeader.Version)))return false;
	if(!f.Write(conv::serialize(mHeader.Checksum)))return false;
	if(!f.Write(conv::serialize(mHeader.NumFiles)))return false;
	for(FilesMap::const_iterator i=mFiles.begin(),e=mFiles.end();i!=e;++i)
	{
		const std::string& Path=i->first.string();	// nem const refet ad vissza, hanem temp-et, tudom. de ez tok valid.
		const FileDesc& Fd=i->second;
		if(!f.Write(conv::serialize((uint32_t)Path.size())))return false;
		if(!f.Write(Path))return false;
		if(!f.Write(conv::serialize(Fd.Offset)))return false;
		if(!f.Write(conv::serialize(Fd.FileSize)))return false;
	}
	return true;
}

PackageWriter::PackageWriter( AutoFile Target )
: PackageBase(Target)
{

}

bool PackageWriter::Save()
{
	if(!mFile.get()||!mFile->IsValid()) return false;
	MemoryFile DataParts;
	uint32_t Offset=0;
	for(PathMap::const_iterator i=mPaths.begin(),e=mPaths.end();i!=e;++i)
	{
		OsFile In(boost::filesystem::absolute(i->first));
		if(!In.IsValid())continue;
		std::string Buffer;
		In.ReadAll(Buffer);	// ez sokszaz megas filenal akar meg fajhat is
		FileDesc& Desc=mFiles[i->second];
		Desc.FileSize=Buffer.size();
		Desc.Offset=Offset;
		Offset+=Desc.FileSize;
		DataParts.Write(Buffer);
	}
	WriteHeader();
	DataParts.CopyTo(*mFile);
	mFile.reset();
	return true;
}

void PackageWriter::Add( const boost::filesystem::path& Path,const boost::filesystem::path& PathInArchive )
{
	mPaths[Path]=PathInArchive;
}

PackageBase::PackageBase( AutoFile F )
: mFile(F)
{

}
