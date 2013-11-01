#ifndef INCLUDED_RENDER_RENDERER_H
#define INCLUDED_RENDER_RENDERER_H
#include "main/window.h"

class Renderer : public Singleton<Renderer>
{
	friend class Singleton<Renderer>;
	Renderer();
	~Renderer();

	uint32_t mWidth;
	uint32_t mHeight;
	float mRatio;
	ModelRepo& mModelRepo;
	Registration mWindowResizeId;

	bool BeginRender();
	bool EndRender();
	void OnWindowResizeEvent(const WindowResizeEvent& Event);
	void Resize(int Width, int Height);
	void SetupRenderer();
public:
	bool Render();
};

#endif//INCLUDED_RENDER_RENDERER_H
