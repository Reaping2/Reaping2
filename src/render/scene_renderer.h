#ifndef INCLUDED_RENDER_SCENE_RENDERER_H
#define INCLUDED_RENDER_SCENE_RENDERER_H

class SceneRenderer
{
	RenderableRepo& mRenderables;
public:
	SceneRenderer();
	void Draw(Scene& Object);
};

#endif//INCLUDED_RENDER_SCENE_RENDERER_H
