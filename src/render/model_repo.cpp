#include "i_render.h"

Model const& ModelRepo::GetModel( Actor const& Object )
{
	ModelMap_t::const_iterator i=mModels.find(Object.GetTypeId());
	if(i==mModels.end()) return mDefaultModel;
	return *(i->second);
}

ModelRepo::ModelRepo()
: mDefaultModel(DefaultModel::Get())
, mPlayerModel(PlayerModel::Get())
{
	mModels.emplace(2,&mPlayerModel);
}

ModelRepo::~ModelRepo()
{
	for(ModelMap_t::const_iterator i=mModels.begin(),e=mModels.end();i!=e;++i)
		delete i->second;
	mModels.clear();
}

