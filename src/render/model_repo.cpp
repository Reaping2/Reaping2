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
	int32_t Id=IdStorage::Get().GetId("player");
	mModels.insert(Id,new PlayerModel);
}

ModelRepo::~ModelRepo()
{
}

