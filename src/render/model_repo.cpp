#include "i_render.h"

Model const& ModelRepo::GetModel( Actor const& Object )
{
	ModelMap_t::const_iterator i=mModels.find(Object.GetTypeId());
	if(i==mModels.end()) return mDefaultModel;
	return *(i->second);
}

ModelRepo::ModelRepo()
: mDefaultModel(DefaultModel::Get())
{
	Model*& m=mModels[2];
	delete m;
	m=new PlayerModel;
}

ModelRepo::~ModelRepo()
{
	for(ModelMap_t::const_iterator i=mModels.begin(),e=mModels.end();i!=e;++i)
		delete i->second;
	mModels.clear();
}

