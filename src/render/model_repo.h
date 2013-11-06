#ifndef INCLUDED_RENDERER_MODEL_REPO_H
#define INCLUDED_RENDERER_MODEL_REPO_H

class Actor;
class Model;
class ModelRepo : public Repository<Model>, public Singleton<ModelRepo>
{
	friend class Singleton<ModelRepo>;
	ModelRepo();
	~ModelRepo();
public:
	Model const& GetModel(Actor const& Object); // i don't think it's good :D more consistent, and you better know what you do.
};

#endif//INCLUDED_RENDERER_MODEL_REPO_H
