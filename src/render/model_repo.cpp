#include "i_render.h"

ModelRepo::ModelRepo()
	: Repository(DefaultModel::Get())
{
	int32_t Id=AutoId("player");
	mElements.insert(Id,new PlayerModel);
}

ModelRepo::~ModelRepo()
{
}

