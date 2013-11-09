#ifndef INCLUDED_RENDERER_MODEL_REPO_H
#define INCLUDED_RENDERER_MODEL_REPO_H

class Actor;
class Model;
class ModelRepo : public Repository<Model>, public Singleton<ModelRepo>
{
	friend class Singleton<ModelRepo>;
	ModelRepo();
	~ModelRepo();
};

#endif//INCLUDED_RENDERER_MODEL_REPO_H
