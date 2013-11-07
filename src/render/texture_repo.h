#ifndef INCLUDED_RENDER_TEXTURE_REPO_H
#define INCLUDED_RENDER_TEXTURE_REPO_H

class TextureRepo : public Repository<Texture>, public Singleton<TextureRepo>
{
	static const Texture DefaultTexture;
	friend class Singleton<TextureRepo>;
	TextureRepo();

	void Init();
	typedef std::vector<int32_t> IntVec_t;
	IntVec_t mUnavailElements;
public:
	virtual Texture const& operator()(int32_t Id);
};

#endif//INCLUDED_RENDER_TEXTURE_REPO_H
