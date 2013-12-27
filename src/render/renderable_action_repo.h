#ifndef INCLUDED_RENDER_RENDERABLE_ACTION_REPO_H
#define INCLUDED_RENDER_RENDERABLE_ACTION_REPO_H

class RenderableActionRepo : public Singleton<RenderableActionRepo>, public Repository<RenderableActions_t>
{
	RenderableActions_t mEmptyActions;
	friend class Singleton<RenderableActionRepo>;
	RenderableActionRepo();
	void Init();
};

#endif//INCLUDED_RENDER_RENDERABLE_ACTION_REPO_H
