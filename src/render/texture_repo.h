#ifndef INCLUDED_RENDER_TEXTURE_REPO_H
#define INCLUDED_RENDER_TEXTURE_REPO_H

class TextureRepo : public Singleton<TextureRepo>
{
	friend class Singleton<TextureRepo>;
	TextureRepo();

	typedef boost::ptr_vector<Texture> Textures_t;
	Textures_t mTextures;

	typedef boost::ptr_map<int32_t, Sprite> SpriteMap_t;
	typedef std::map<int32_t, SpriteMap_t> ActorMap_t;
	ActorMap_t mActorTypes;

	void Init();
	static bool AddTexDesc(Json::Value& TexDesc, Package& Pkg, Textures_t& Textures, ActorMap_t& ActorTypes);
	static bool AddSpriteMap(Json::Value& SpriteDesc,Texture* Tex,ActorMap_t& ActorTypes);
	static bool AddSprite(Json::Value& SpriteDesc,Texture* Tex, uint32_t W, uint32_t H, SpriteMap_t& SpriteMap);
public:
	Sprite const* GetSprite(int32_t TypeId, int32_t ActionId) const;
};

#endif//INCLUDED_RENDER_TEXTURE_REPO_H
