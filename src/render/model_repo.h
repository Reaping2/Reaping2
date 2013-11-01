#ifndef INCLUDED_RENDERER_MODEL_REPO_H
#define INCLUDED_RENDERER_MODEL_REPO_H

class Actor;
class Model;
class ModelRepo : public Singleton<ModelRepo>
{
	typedef boost::ptr_map<int32_t, Model> ModelMap_t;
	ModelMap_t mModels;
	DefaultModel& mDefaultModel;

	friend class Singleton<ModelRepo>;
	ModelRepo();
	~ModelRepo();
public:
	Model const& GetModel(Actor const& Object);
};

#endif//INCLUDED_RENDERER_MODEL_REPO_H
