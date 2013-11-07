#include "i_render.h"

Model const& ModelRepo::GetModel( Actor const& Object )
{
	return operator()(Object.GetId());
}

ModelRepo::ModelRepo()
	: Repository(DefaultModel::Get())
{
	int32_t Id=AutoId("player");
	mElements.insert(Id,new PlayerModel);
}

ModelRepo::~ModelRepo()
{
}

