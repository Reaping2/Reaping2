#include "i_render.h"

RenderableActionRepo::RenderableActionRepo()
:Repository<RenderableActions_t>(mEmptyActions)
{
	Init();
}

void RenderableActionRepo::Init()
{
	Filesys& Fsys(Filesys::Get());
	AutoFile F(Fsys.Open("renderable_actions.json"));
	if(!F.get()||!F->IsValid())return;
	JsonReader R(*F);
	if(!R.IsValid())
	{
		assert(false);
		return;
	}
	Json::Value Root=R.GetRoot();
	assert(Root.isArray());
	for(Json::Value::iterator i=Root.begin(),e=Root.end();i!=e;++i)
	{
		Json::Value& Elem=*i;
		assert(Elem.isArray());
		if(!Elem.isArray())continue;
		Json::Value& Key=Elem[0];
		Json::Value& Values=Elem[1];
		assert(Values.isArray());
		if(!Values.isArray())continue;
		std::string Str;
		if(!Json::GetStr(Key,Str))continue;
		int32_t IKey=AutoId(Str);
		std::auto_ptr<RenderableActions_t> Actions(new RenderableActions_t);
		for(Json::Value::iterator j=Values.begin(),k=Values.end();j!=k;++j)
		{
			Json::Value& Val=*j;
			if(!Json::GetStr(Val,Str))continue;
			Actions->push_back(AutoId(Str));
		}
		mElements.insert(IKey,Actions);
	}
}
