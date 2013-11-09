#include "i_render.h"

RenderableRepo::RenderableRepo()
: RepoBase(DefaultRenderable)
{
	Init();
}

bool RenderableRepo::AddSpritesFromOneTextureDesc(Json::Value& TexDesc, ElementMap_t& Renderables)
{
	std::string PathStr;
	Json::Value& ActorVisuals=TexDesc["actor_visuals"];
	if(!ActorVisuals.isArray())return false;
	if(!Json::GetStr(TexDesc["texture_path"],PathStr))return false;
	int32_t TexId=AutoId(boost::filesystem::path(PathStr).generic_string());
	const size_t NumActors=ActorVisuals.size();
	for(size_t i=0;i<NumActors;++i)
	{
		Json::Value& ActorVisualDesc=ActorVisuals[i];
		if(!ActorVisualDesc.isObject())return false;
		std::auto_ptr<SpriteCollection> Renderable(new SpriteCollection);
		if(!Renderable->Load(TexId,ActorVisualDesc))return false;
		int32_t Id=Renderable->Id();
		ElementMap_t::iterator It=Renderables.find(Id);
		if(It==Renderables.end())
			Renderables.insert(Id,Renderable.release());
		else
			It->second->Merge(*Renderable);
	}
	return true;
}

void RenderableRepo::Init()
{
	Package::PathVect_t Paths;
	Filesys& FSys=Filesys::Get();
	FSys.GetFileNames(Paths,"sprites");
	ElementMap_t Renderables;
	for(Package::PathVect_t::const_iterator i=Paths.begin(),e=Paths.end();i!=e;++i)
	{
		boost::filesystem::path const& Path=*i;
		if(Path.extension().string()!=".json")continue;
		AutoFile JsonFile=FSys.Open(*i);
		if(!JsonFile.get())continue;
		JsonReader Reader(*JsonFile);
		if(!Reader.IsValid())continue;
		Json::Value Root=Reader.GetRoot();
		if(!Root.isArray())continue;
		size_t NumTextures=Root.size();
		for(size_t i=0;i<NumTextures;++i)
		{
			Json::Value& TexDesc=Root[i];
			if(!AddSpritesFromOneTextureDesc(TexDesc,Renderables))return;
		}
	}
	// all done
	using std::swap;
	swap(mElements,Renderables);
}

SpriteCollection RenderableRepo::DefaultRenderable=SpriteCollection();

