#include "i_render.h"

bool TextureRepo::AddAnimation(Json::Value& AnimationDesc,Texture* Tex, uint32_t W, uint32_t H, AnimationMap_t& AnimationMap)
{
	std::string AnimationName;
	if(!Json::GetStr(AnimationDesc["name"],AnimationName))return false;
	uint32_t StartX, StartY, NumPhases;
	if(!Json::GetUInt(AnimationDesc["start_x"],StartX)||
		!Json::GetUInt(AnimationDesc["start_y"],StartY)||
		!Json::GetUInt(AnimationDesc["num_phases"],NumPhases))
		return false;
	int32_t ActionId=IdStorage::Get().GetId(AnimationName);
	AnimationMap.insert(ActionId,new Sprite(Tex,W,H,StartX,StartY,NumPhases));
	return true;
}

bool TextureRepo::AddActorVisualDesc(Json::Value& ActorVisualDesc,Texture* Tex,ActorMap_t& ActorVisuals)
{
	std::string NameStr;
	if(!Json::GetStr(ActorVisualDesc["name"],NameStr))return false;
	uint32_t W,H;
	if(!Json::GetUInt(ActorVisualDesc["w"],W)||
		!Json::GetUInt(ActorVisualDesc["h"],H))return false;
	Json::Value Anims=ActorVisualDesc["animations"];
	if(!Anims.isArray())return false;
	const size_t NumAnims=Anims.size();
	AnimationMap_t& AnimationMap=ActorVisuals[IdStorage::Get().GetId(NameStr)];
	for(size_t i=0;i<NumAnims;++i)
	{
		Json::Value& Anim=Anims[i];
		if(!Anim.isObject())return false;
		if(!AddAnimation(Anim,Tex,W,H,AnimationMap))return false;
	}
	return true;
}

bool TextureRepo::AddSpritesFromOneTextureDesc(Json::Value& TexDesc, Package& Pkg, Textures_t& Textures, ActorMap_t& ActorTypes)
{
	std::string PathStr;
	if(!Json::GetStr(TexDesc["texture_path"],PathStr))return false;
	AutoFile TexFile=Pkg.Open(PathStr);
	if(!TexFile.get())return false;
	PngTexture Png(*TexFile);
	if(!Png.IsValid())return false;
	Texture* Tex=new Texture(Png.GetWidth(),Png.GetHeight(),Png.GetChannels(),Png.GetData());
	Textures.push_back(Tex);
	Json::Value& ActorVisuals=TexDesc["actor_visuals"];
	if(!ActorVisuals.isArray())return false;
	const size_t NumActors=ActorVisuals.size();
	for(size_t i=0;i<NumActors;++i)
	{
		Json::Value& ActorVisualDesc=ActorVisuals[i];
		if(!ActorVisualDesc.isObject())return false;
		if(!AddActorVisualDesc(ActorVisualDesc,Tex,ActorTypes))return false;
	}
	return true;
}

void TextureRepo::Init()
{
	Package Pkg(AutoFile(new OsFile("data.pkg")));
	Package::PathVect_t Paths;
	Pkg.GetFileNames(Paths,"textures");
	Textures_t Textures;
	ActorMap_t ActorTypes;
	for(Package::PathVect_t::const_iterator i=Paths.begin(),e=Paths.end();i!=e;++i)
	{
		boost::filesystem::path const& Path=*i;
		if(Path.extension().string()!=".json") continue;
		AutoFile JsonFile=Pkg.Open(*i);
		if(!JsonFile.get())return;
		JsonReader Reader(*JsonFile);
		if(!Reader.IsValid())return;
		Json::Value Root=Reader.GetRoot();
		if(!Root.isArray())return;
		size_t NumTextures=Root.size();
		for(size_t i=0;i<NumTextures;++i)
		{
			Json::Value& TexDesc=Root[i];
			if(!AddSpritesFromOneTextureDesc(TexDesc,Pkg,Textures,ActorTypes))return;
		}
	}
	// all done
	using std::swap;
	swap(mTextures,Textures);
	swap(mActorTypes,ActorTypes);
}

TextureRepo::TextureRepo()
{
	Init();
}

Sprite const* TextureRepo::GetSprite(int32_t TypeId, int32_t ActionId) const
{
	ActorMap_t::const_iterator i=mActorTypes.find(TypeId);
	if(i==mActorTypes.end())return NULL;
	AnimationMap_t const& SpMap=i->second;
	AnimationMap_t::const_iterator i2=SpMap.find(ActionId);
	if(i2==SpMap.end())return NULL;
	return i2->second;
}

