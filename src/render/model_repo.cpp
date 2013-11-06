#include "i_render.h"

Model const& ModelRepo::GetModel( Actor const& Object )
{
	return GetById(Object.GetId());
}

ModelRepo::ModelRepo()
	: Repository(DefaultModel::Get())
{
	int32_t Id=IdStorage::Get().GetId("player");
	mElements.insert(Id,new PlayerModel);
}

ModelRepo::~ModelRepo()
{
}

