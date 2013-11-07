#include "i_render.h"

void TextureRepo::Init()
{
	Package Pkg(AutoFile(new OsFile("data.pkg")));
	Package::PathVect_t Paths;
	Pkg.GetFileNames(Paths,"textures");
	for(Package::PathVect_t::const_iterator i=Paths.begin(),e=Paths.end();i!=e;++i)
	{
		boost::filesystem::path const& Path=*i;
		if(Path.extension().string()!=".png")continue;
		AutoFile TexFile=Pkg.Open(Path);
		if(!TexFile.get())continue;
		PngTexture Png(*TexFile);
		if(!Png.IsValid())continue;
		Texture* Tex=new Texture(Png.GetWidth(),Png.GetHeight(),Png.GetChannels(),Png.GetData());
		int32_t Id=AutoId(Path.generic_string());
		mElements.insert(Id,Tex);
	}
}

TextureRepo::TextureRepo()
: RepoBase(DefaultTexture)
{
	// Init();
}

Texture const& TextureRepo::operator()(int32_t Id)
{
	ElementMap_t::const_iterator i=mElements.find(Id);
	if(i!=mElements.end()) return *(i->second);
	if(std::find(mUnavailElements.begin(),mUnavailElements.end(),Id)!=mUnavailElements.end()) return mDefaultElement;
	do
	{
		std::string Path;
		if(!IdStorage::Get().GetName(Id,Path))break;
		Package Pkg(AutoFile(new OsFile("data.pkg")));
		AutoFile TexFile=Pkg.Open(Path);
		if(!TexFile.get())break;
		PngTexture Png(*TexFile);
		if(!Png.IsValid())break;
		std::auto_ptr<Texture> Tex(new Texture(Png.GetWidth(),Png.GetHeight(),Png.GetChannels(),Png.GetData()));
		if(Tex->TexId()==0)break;
		mElements.insert(Id,Tex.get());
		return *Tex.release();
	}while(false);
	mUnavailElements.push_back(Id);
	return DefaultTexture;
}

const Texture TextureRepo::DefaultTexture=Texture(1,1,0,NULL);
