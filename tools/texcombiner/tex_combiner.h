#ifndef INCLUDED_TEXCOMBINER_TEX_COMBINER_H
#define INCLUDED_TEXCOMBINER_TEX_COMBINER_H

class TexCombiner
{
	static const uint16_t mPadding;
	struct PhaseDesc
	{
		std::string action;
		uint32_t phase;
		uint32_t total;
		PhaseDesc(std::string const& a, uint32_t p, uint32_t t)
			:action(a),phase(p),total(t){}
	};
	typedef std::vector<PhaseDesc> Phases_t;
	struct TexDesc
	{
		TextureBase const* tex;
		uint16_t w;
		uint16_t h;
		std::string actor;
		Phases_t phases;
		uint16_t phase;
		double scale;
	};
	struct Region
	{
		uint16_t x;
		uint16_t y;
		uint16_t w;
		uint16_t h;
		std::auto_ptr<Region> small;
		std::auto_ptr<Region> big;
		TexDesc const* Tex;
		Region():Tex(NULL){}
		bool Place(TexDesc const* NewTex);
		void Render(uint8_t* Data,uint16_t TotalWidth,uint16_t TotalHeight);
		void AddToJson(Json::Value& ActorVisuals);
	private:
		Json::Value& FindActor(Json::Value& ActorVisuals);
		Json::Value& FindAnimation(Json::Value& Animations, PhaseDesc const& Anim);
	};
	struct TextureSorter
	{
		bool operator()(TexDesc const& t1, TexDesc const& t2);
	};
	typedef boost::ptr_vector<TextureBase> Textures_t;
	Textures_t mTextures;
	typedef boost::ptr_vector<TexDesc> TextureDescs_t;
	TextureDescs_t mDescs;
	typedef std::map<std::string,std::string> Checker_t;
	Checker_t mChecker;
	Region mRoot;
	std::auto_ptr<uint8_t> mData;
public:
	void SetDestinationSize(uint16_t Width, uint16_t Height);
	void AddTexture(boost::filesystem::path const& NewTex);
	bool Save(std::string const& Name);
};

#endif//INCLUDED_TEXCOMBINER_TEX_COMBINER_H
