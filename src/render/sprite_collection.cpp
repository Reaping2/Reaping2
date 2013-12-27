#include "i_render.h"

SpriteCollection::SpriteCollection()
: RepoBase(DefaultSprite)
, mId(0)
{

}

int32_t SpriteCollection::Id() const
{
	return mId;
}

bool SpriteCollection::Load(int32_t TexId, Json::Value& Root)
{
	std::string NameStr;
	if(!Json::GetStr(Root["name"],NameStr))return false;
	Json::Value Anims=Root["animations"];
	if(!Anims.isArray())return false;
	const size_t NumAnims=Anims.size();
	ElementMap_t Animations;
	for(size_t i=0;i<NumAnims;++i)
	{
		Json::Value& Anim=Anims[i];
		if(!Anim.isObject())return false;
		std::string AnimationName;
		if(!Json::GetStr(Anim["name"],AnimationName))return false;
		int32_t ActionId=AutoId(AnimationName);
		Sprite* Spr=new Sprite(TexId,Anim);
		Animations.insert(ActionId,Spr);
	}
	// all is good
	mId=AutoId(NameStr);
	mElements.transfer(Animations.begin(),Animations.end(),Animations);
	return true;
}

void SpriteCollection::Merge( SpriteCollection& Other )
{
	assert(mId==Other.mId);
	mElements.transfer(Other.mElements.begin(),Other.mElements.end(),Other.mElements);
}

Sprite SpriteCollection::DefaultSprite=Sprite(NULL,Json::Value());
