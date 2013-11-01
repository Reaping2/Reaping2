#include "i_render.h"

bool TextureRepo::AddSprite(Json::Value& SpriteDesc,Texture* Tex, uint32_t W, uint32_t H, SpriteMap_t& SpriteMap)
{
	std::string SpriteActionName;
	if(!Json::GetStr(SpriteDesc["name"],SpriteActionName))return false;
	uint32_t StartX, StartY, NumPhases;
	if(!Json::GetUInt(SpriteDesc["sx"],StartX)||
		!Json::GetUInt(SpriteDesc["sy"],StartY)||
		!Json::GetUInt(SpriteDesc["num_phases"],NumPhases))
		return false;
	int32_t TodoActionId=IdStorage::Get().GetId(SpriteActionName);
	SpriteMap.insert(TodoActionId,new Sprite(Tex,W,H,StartX,StartY,NumPhases));
	return true;
}

bool TextureRepo::AddSpriteMap(Json::Value& SpriteMapDesc,Texture* Tex,ActorMap_t& ActorTypes)
{
	std::string NameStr;
	if(!Json::GetStr(SpriteMapDesc["name"],NameStr))return false;
	uint32_t W,H;
	if(!Json::GetUInt(SpriteMapDesc["w"],W)||
		!Json::GetUInt(SpriteMapDesc["h"],H))return false;
	Json::Value Anims=SpriteMapDesc["anims"];
	if(!Anims.isArray())return false;
	const size_t NumAnims=Anims.size();
	SpriteMap_t& SpriteMap=ActorTypes[IdStorage::Get().GetId(NameStr)];
	for(size_t i=0;i<NumAnims;++i)
	{
		Json::Value& Anim=Anims[i];
		if(!Anim.isObject())return false;
		if(!AddSprite(Anim,Tex,W,H,SpriteMap))return false;
	}
	return true;
}

bool TextureRepo::AddTexDesc(Json::Value& TexDesc, Package& Pkg, Textures_t& Textures, ActorMap_t& ActorTypes)
{
	std::string PathStr;
	if(!Json::GetStr(TexDesc["texture_path"],PathStr))return false;
	AutoFile TexFile=Pkg.Open(PathStr);
	if(!TexFile.get())return false;
	PngTexture Png(*TexFile);
	if(!Png.IsValid())return false;
	Texture* Tex=new Texture(Png.GetWidth(),Png.GetHeight(),Png.GetChannels(),Png.GetData());
	Textures.push_back(Tex);
	Json::Value& SpriteDescs=TexDesc["textures"];
	if(!SpriteDescs.isArray())return false;
	const size_t NumActors=SpriteDescs.size();
	for(size_t i=0;i<NumActors;++i)
	{
		Json::Value& SpriteDesc=SpriteDescs[i];
		if(!SpriteDesc.isObject())return false;
		if(!AddSpriteMap(SpriteDesc,Tex,ActorTypes))return false;
	}
	return true;
}

void TextureRepo::Init()
{
	Package Pkg(AutoFile(new OsFile("data.pkg")));
	AutoFile JsonFile=Pkg.Open("textures.json");
	if(!JsonFile.get())return;
	JsonReader Reader(*JsonFile);
	if(!Reader.IsValid())return;
	Json::Value Root=Reader.GetRoot();
	if(!Root.isArray())return;
	size_t NumTextures=Root.size();
	Textures_t Textures;
	ActorMap_t ActorTypes;
	for(size_t i=0;i<NumTextures;++i)
	{
		Json::Value& TexDesc=Root[i];
		if(!AddTexDesc(TexDesc,Pkg,Textures,ActorTypes))return;
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
	SpriteMap_t const& SpMap=i->second;
	SpriteMap_t::const_iterator i2=SpMap.find(ActionId);
	if(i2==SpMap.end())return NULL;
	return i2->second;
}

