#ifndef INCLUDED_RENDER_RENDERER_H
#define INCLUDED_RENDER_RENDERER_H

class Renderer : public Singleton<Renderer>
{
	friend class Singleton<Renderer>;
	Renderer();
	~Renderer();

	uint32_t mWidth;
	uint32_t mHeight;
	float mRatio;
	ModelRepo& mModelRepo;

	bool BeginRender();
	bool EndRender();
	void SetDimensions(const uint32_t Width, const uint32_t Height);
public:
	bool Render();
};

#endif//INCLUDED_RENDER_RENDERER_H
