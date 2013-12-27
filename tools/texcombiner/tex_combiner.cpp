#include "i_texcombiner.h"

bool TexCombiner::Region::Place(TexDesc const* NewTex )
{
	uint16_t const tw=NewTex->w+2*mPadding;
	uint16_t const th=NewTex->h+2*mPadding;

	if(tw>w||th>h)	// todo: allow rotate?
		return false;
	if(!Tex)
	{
		Tex=NewTex;
		small.reset(new Region);
		big.reset(new Region);
		// split
		uint16_t Rw=w-tw;
		uint16_t Rh=h-th;
		if(w+Rh>=h+Rw)
		{
			big->w=w;
			big->h=Rh;
			big->x=x;
			big->y=y+th;
			small->w=Rw;
			small->h=th;
			small->x=x+tw;
			small->y=y;
		}
		else
		{
			big->w=Rw;
			big->h=h;
			big->x=x+tw;
			big->y=y;
			small->w=tw;
			small->h=Rh;
			small->x=x;
			small->y=y+th;
		}
		return true;
	}
	return small->Place(NewTex)||big->Place(NewTex);
}

void TexCombiner::Region::Render( uint8_t* Data,uint16_t TotalWidth,uint16_t TotalHeight )
{
	if(!Tex||!Tex->tex)return;
	// draw tex
	uint8_t const* TexData=Tex->tex->GetData();
	uint16_t const th=Tex->tex->GetHeight();
	uint16_t const tw=Tex->tex->GetWidth();
	// TextureBase is 4 channel, so no conversion is necessary
	for(uint16_t j=0;j<Tex->h;++j)
	{
//		uint8_t* Dst=&Data[4*(x+mPadding)+4*(TotalHeight-(1+y+mPadding+j))*TotalWidth];
		uint8_t* Dst=&Data[4*(x+mPadding)+4*(y+mPadding+j)*TotalWidth];
		uint8_t const* Src=&TexData[4*(j*tw+Tex->phase*Tex->w)];
		memcpy(Dst,Src,4*Tex->w);
		TgaTexture::Reorder(Dst,Tex->w);
	}
	small->Render(Data,TotalWidth,TotalHeight);
	big->Render(Data,TotalWidth,TotalHeight);
}

Json::Value& TexCombiner::Region::FindActor( Json::Value& ActorVisuals )
{
	for(Json::Value::iterator i=ActorVisuals.begin(),e=ActorVisuals.end();i!=e;++i)
	{
		Json::Value& I=*i;
		if(I["name"]==Tex->actor)
			return I;
	}
	Json::Value NewActor(Json::objectValue);
	NewActor["name"]=Json::Value(Tex->actor);
	NewActor["animations"]=Json::Value(Json::arrayValue);
	ActorVisuals.append(NewActor);
	return ActorVisuals[ActorVisuals.size()-1];
}

void TexCombiner::Region::AddToJson( Json::Value& ActorVisuals )
{
	if(!Tex)
		return;

	Json::Value& AnimsNode=FindActor(ActorVisuals)["animations"];
	for(Phases_t::const_iterator i=Tex->phases.begin(),e=Tex->phases.end();i!=e;++i)
	{
		PhaseDesc const& Ph=*i;
		Json::Value& AnimNode=FindAnimation(AnimsNode,Ph);
		Json::Value& Phase=AnimNode["phases"][Ph.phase];
		Phase["x"]=Json::Value(x+mPadding);
		Phase["y"]=Json::Value(y+mPadding);
	}
	small->AddToJson(ActorVisuals);
	big->AddToJson(ActorVisuals);
}

Json::Value& TexCombiner::Region::FindAnimation( Json::Value& Animations, PhaseDesc const& Anim )
{
	for(Json::Value::iterator i=Animations.begin(),e=Animations.end();i!=e;++i)
	{
		Json::Value& I=*i;
		if(I["name"]==Anim.action)
			return I;
	}
	Json::Value NewAnim(Json::objectValue);
	NewAnim["name"]=Json::Value(Anim.action);
	NewAnim["w"]=Json::Value(Tex->w);
	NewAnim["h"]=Json::Value(Tex->h);
	NewAnim["scale"]=Json::Value(Tex->scale);
	NewAnim["phases"].resize(Anim.total);
	Animations.append(NewAnim);
	return Animations[Animations.size()-1];
}

void TexCombiner::SetDestinationSize( uint16_t Width, uint16_t Height )
{
	mData.reset(new uint8_t[Width*Height*4]);
	memset(mData.get(),0,Width*Height*4);
	mRoot.x=0;
	mRoot.y=0;
	mRoot.w=Width;
	mRoot.h=Height;
}

bool TexCombiner::Save( std::string const& Name )
{
	std::sort(mDescs.begin(),mDescs.end(),TextureSorter());
	for(TextureDescs_t::const_iterator i=mDescs.begin(),e=mDescs.end();i!=e;++i)
	{
		if(!mRoot.Place(&*i))
		{
			break;
			//assert(false);
			//return false;
		}
	}
	mRoot.Render(mData.get(),mRoot.w,mRoot.h);
	OsFile Out(Name+".tga",std::ios_base::out);
	if(!Out.IsValid())return false;
	std::string HeaderData(18,'\0');
	uint8_t HD[18]={};
	HD[2]=2;
	HD[12]=mRoot.w&0xff; HD[13]=(mRoot.w>>8)&0xff;
	HD[14]=mRoot.h&0xff; HD[15]=(mRoot.h>>8)&0xff;
	HD[16]=32;
	HD[17]=8;
	Out.Write(HD,18);
	Out.Write(mData.get(),4*mRoot.w*mRoot.h);
	uint8_t Footer[26]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x54,0x52,0x55,0x45,0x56,0x49,0x53,0x49,0x4f,0x4e,0x2d,0x58,0x46,0x49,0x4c,0x45,0x2e,0x00};
	Out.Write(Footer,26);
	//prepare json
	Json::Value OneTextureFile(Json::objectValue);
	Json::Value Root(Json::arrayValue);
	OneTextureFile["texture_path"]=Json::Value("textures/"+Name+".tga");
	mRoot.AddToJson(OneTextureFile["actor_visuals"]);
	Root.append(OneTextureFile);
	//write
	Json::StyledWriter Writer;
	std::string const& JString=Writer.write(Root);
	OsFile OutJson(Name+".json",std::ios_base::out);
	OutJson.Write(JString);
	return true;
}

void TexCombiner::AddTexture(boost::filesystem::path const& NewTex)
{
	boost::filesystem::path const& Ext=NewTex.extension();
	OsFile f(NewTex);
	if(!f.IsValid())return;
	std::auto_ptr<TextureBase> TexBase;
	if(boost::iequals(Ext.string(),".png"))
		TexBase.reset(new PngTexture(f));
	else if(boost::iequals(Ext.string(),".tga"))
		TexBase.reset(new TgaTexture(f));
	else
		return;

	boost::filesystem::path Desc=NewTex;
	Desc.replace_extension(".json");
	static bool CreateDummyJsonFiles=false;
	static bool Overwrite=false;
	if(CreateDummyJsonFiles&&(Overwrite||!boost::filesystem::exists(Desc)))
	{
		OsFile j2(Desc,std::ios_base::out);
		j2.Write(std::string("[\"\",1,1.0,[\"default_action\",0,1]]"));
		return;
	}
	OsFile j(Desc);
	if(!j.IsValid())return;
	JsonReader Reader(j);
	if(!Reader.IsValid())return;
	Json::Value Root=Reader.GetRoot();
	if(!Root.isArray()||Root.size()<4)return;
	std::string actor;
	uint32_t phases;
	if(!Json::GetStr(Root[0],actor)||
		!Json::GetUInt(Root[1],phases))
		return;

	double scale;
	if(!Json::GetDouble(Root[2],scale))
	{
//#define TRANSITION
#ifdef TRANSITION
		Root.append(Root[2]);
		Root[2]=Json::Value(1.0);
		Json::FastWriter Writer;
		std::string const& JString=Writer.write(Root);
		Desc.replace_extension(".json2");
		OsFile OutJson(Desc,std::ios_base::out);
		OutJson.Write(JString);
#endif//TRANSITION
		return;
	}
	if(actor.empty()||!phases)
		return;

	uint16_t const w=TexBase->GetWidth();
	uint16_t const h=TexBase->GetHeight();
	std::vector<Phases_t> Phases(phases);
	for(size_t i=3,e=Root.size();i<e;++i)
	{
		Json::Value& JAction=Root[i];
		if(!JAction.isArray()||JAction.size()!=3)return;
		std::string action;
		uint32_t from, to;
		if(!Json::GetStr(JAction[0],action)||
			!Json::GetUInt(JAction[1],from)||
			!Json::GetUInt(JAction[2],to))
			return;

		std::pair<Checker_t::iterator,bool> Check=mChecker.insert(std::make_pair(actor+"."+action,Desc.string()));
		if(!Check.second)
		{
			assert(false);
			LOG("Repeated action/actor: %s, %s, %s",Check.first->first.c_str(),Check.first->second.c_str(),Desc.string().c_str());
		}

		for(uint32_t p=from,t=to-from;p!=to;++p)
			Phases[p].push_back(PhaseDesc(action,p-from,t));
	}
	// all good
	for(uint32_t i=0;i<phases;++i)
	{
		TexDesc* d=new TexDesc;
		d->actor=actor;
		d->tex=TexBase.get();
		d->h=h;
		d->w=w/phases;
		d->phase=i;
		d->scale=scale;
		using std::swap;
		swap(d->phases,Phases[i]);
		if(!d->phases.empty())
			mDescs.push_back(d);
		else
			delete d;
	}
	mTextures.push_back(TexBase);
}

const uint16_t TexCombiner::mPadding=2;

bool TexCombiner::TextureSorter::operator()(TexDesc const& t1, TexDesc const& t2)
{
	return t1.h+t1.w>t2.h+t2.w;
}
