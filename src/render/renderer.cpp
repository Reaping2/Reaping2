#include "i_render.h"

Renderer::Renderer()
: mWorldProjector(-1.0f,1.0f)
, mUiProjector(0.0f,100.0f,Projection::VM_Fixed)
, mCamera(mWorldProjector)
, mUi(Ui::Get())
, mParticleEngine(ParticleEngine::Get())
, mShaderManager(ShaderManager::Get())
{
	Font::Get();
	mMouseMoveId=EventServer<ScreenMouseMoveEvent>::Get().Subscribe(boost::bind(&Renderer::OnMouseMoveEvent,this,_1));
	mMousePressId=EventServer<ScreenMousePressEvent>::Get().Subscribe(boost::bind(&Renderer::OnMousePressEvent,this,_1));
	mMouseReleaseId=EventServer<ScreenMouseReleaseEvent>::Get().Subscribe(boost::bind(&Renderer::OnMouseReleaseEvent,this,_1));
	Init();
}

Renderer::~Renderer()
{

}

void Renderer::RenderActors(bool Alive)
{
	Scene& Scen=Scene::Get();
	const ActorList_t& Lst=Scen.GetActors();
	for(ActorList_t::const_iterator i=Lst.begin(),e=Lst.end();i!=e;++i)
	{
		const Actor& Object=*i;
		if(Alive!=Object.IsAlive())continue;
		static ModelRepo const& Models(ModelRepo::Get());
		Model const& Model=Models(Object.GetId());
		Model.Draw(Object);
	}
}

void Renderer::RenderSprites()
{
	Scene& Scen=Scene::Get();
	const ActorList_t& Lst=Scen.GetActors();
	if(Lst.empty())return;
	GLuint TexId=-1;
	static size_t PrevVecSize=0;
	struct RenderableSprite
	{
		Actor const* Obj;
		SpritePhase const* Spr;
		RenderableSprite(Actor const*o,SpritePhase const*s)
			:Obj(o),Spr(s){}
	};
	typedef std::vector<RenderableSprite> RenderableSprites_t;
	RenderableSprites_t RenderableSprites;
	RenderableSprites.reserve(PrevVecSize);
	for(ActorList_t::const_iterator i=Lst.begin(),e=Lst.end();i!=e;++i)
	{
		const Actor& Object=*i;
		Actor::ActionList_t const& Actions=Object.GetActions();
		static RenderableRepo& Rend(RenderableRepo::Get());
		SpriteCollection const& Sprites=Rend(Object.GetId());
		for(Actor::ActionList_t::const_iterator i=Actions.begin(),e=Actions.end();i!=e;++i)
		{
			Action const& Act=*i->second;
			Sprite const& Spr=Sprites(Act.GetId());
			if(!Spr.IsValid()) continue;
			SpritePhase const& Phase=Spr((int32_t)Act.GetState());
			//for(size_t test=0;test<100;++test)
				RenderableSprites.push_back(RenderableSprite(&Object,&Phase));
		}
		Actor::ItemList_t const& items=Object.GetItems();
		for(Actor::ItemList_t::const_iterator i=items.begin(),e=items.end();i!=e;++i)
		{
			Item const& Act=*i;
			Sprite const& Spr=Sprites(Act.GetId());
			if(!Spr.IsValid()) continue;
			SpritePhase const& Phase=Spr((int32_t)Act.GetState());
			//for(size_t test=0;test<100;++test)
				RenderableSprites.push_back(RenderableSprite(&Object,&Phase));
		}
	}

	// TODO: sort Z order, alive state es texture id alapjan.
	// Meg persze (last cmp) pointerek szerint, hogy determinisztikus legyen.

	size_t CurSize=RenderableSprites.size();
	typedef std::vector<glm::vec2> Positions_t;
	Positions_t Positions;
	Positions.reserve(CurSize);
	typedef std::vector<GLfloat> Floats_t;
	Floats_t Headings;
	Headings.reserve(CurSize);
	Floats_t Sizes;
	Sizes.reserve(CurSize);
	typedef std::vector<glm::vec4> TexCoords_t;
	TexCoords_t TexCoords;
	TexCoords.reserve(CurSize);

	for(RenderableSprites_t::const_iterator i=RenderableSprites.begin(),e=RenderableSprites.end();i!=e;++i)
	{
		Positions.push_back(glm::vec2(i->Obj->GetX(),i->Obj->GetY()));
		Headings.push_back(i->Obj->GetOrientation());
		Sizes.push_back(i->Obj->GetRadius());
		TexCoords.push_back(glm::vec4(i->Spr->Left,i->Spr->Right,i->Spr->Bottom,i->Spr->Top));
	}

	if(CurSize==0)
		return;

	mVAO.Bind();
	if(CurSize!=PrevVecSize)
	{
		PrevVecSize=CurSize;
		size_t TotalSize=CurSize*(sizeof(glm::vec4)+sizeof(glm::vec2)+2*sizeof(GLfloat));
		glBufferData(GL_ARRAY_BUFFER, TotalSize, NULL, GL_DYNAMIC_DRAW);
	}

	TexId=RenderableSprites.begin()->Spr->TexId;
	size_t CurrentOffset=0;
	size_t CurrentSize=CurSize*sizeof(glm::vec4);
	GLuint CurrentAttribIndex=0;
	glBufferSubData(GL_ARRAY_BUFFER,CurrentOffset,CurrentSize,&TexCoords[0]);
	glEnableVertexAttribArray(CurrentAttribIndex);
	glVertexAttribPointer(CurrentAttribIndex, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)CurrentOffset);
	glVertexAttribDivisor(CurrentAttribIndex, 1);
	++CurrentAttribIndex;

	CurrentOffset+=CurrentSize;
	CurrentSize=CurSize*sizeof(glm::vec2);
	glBufferSubData(GL_ARRAY_BUFFER,CurrentOffset,CurrentSize,&Positions[0]);
	glEnableVertexAttribArray(CurrentAttribIndex);
	glVertexAttribPointer(CurrentAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)CurrentOffset);
	glVertexAttribDivisor(CurrentAttribIndex, 1);
	++CurrentAttribIndex;

	CurrentOffset+=CurrentSize;
	CurrentSize=CurSize*sizeof(GLfloat);
	glBufferSubData(GL_ARRAY_BUFFER,CurrentOffset,CurrentSize,&Headings[0]);
	glEnableVertexAttribArray(CurrentAttribIndex);
	glVertexAttribPointer(CurrentAttribIndex, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid*)CurrentOffset);
	glVertexAttribDivisor(CurrentAttribIndex, 1);
	++CurrentAttribIndex;

	CurrentOffset+=CurrentSize;
	CurrentSize=CurSize*sizeof(GLfloat);
	glBufferSubData(GL_ARRAY_BUFFER,CurrentOffset,CurrentSize,&Sizes[0]);
	glEnableVertexAttribArray(CurrentAttribIndex);
	glVertexAttribPointer(CurrentAttribIndex, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid*)CurrentOffset);
	glVertexAttribDivisor(CurrentAttribIndex, 1);

	Shader const& Shad(ShaderRepo::Get()(AutoId("sprite2")));
	Shad.Bind();
	glActiveTexture(GL_TEXTURE0+1);
	glBindTexture(GL_TEXTURE_2D,TexId);
	GLuint Unif=glGetUniformLocation(Shad.GetProgId(),"spriteTexture");
	glUniform1i(Unif,1);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP,0,4,CurSize);
	Shad.Unbind();
	glActiveTexture(GL_TEXTURE0);
	mVAO.Unbind();
}

bool Renderer::Render()
{
	if(!BeginRender()) return false;
	mCamera.Update();

	// render world
	SetupRenderer(mWorldProjector);
	mSceneRenderer.Draw(Scene::Get());
	mParticleEngine.Draw(ParticleEngine::GroundParticle);
	RenderSprites();
//	RenderableRepo::Get().Start();
//	RenderActors(false);
//	RenderActors(true);
	// render ui
	SetupRenderer(mUiProjector);

	for(Widget::const_iterator i=mUi.GetRoot().begin(),e=mUi.GetRoot().end();i!=e;++i)
	{
		static UiModelRepo const& UiModels(UiModelRepo::Get());
		Widget const& Wdg=*i;
		if(!(int32_t)Wdg(Widget::PT_Visible)) continue;
		UiModel const& Model=UiModels(Wdg.GetId());
		Model.Draw(Wdg);
	}
	return EndRender();
}

void Renderer::SetupRenderer(const Projection& Proj)
{
	Viewport const& Vp=Proj.GetViewport();
	glViewport(Vp.X, Vp.Y, Vp.Width, Vp.Height);
	glMatrixMode(GL_PROJECTION);
	// todo: ez nyilvan shader programba kerul atzuzasra, a c kod csak a matrixot ismeri
	// glmatrixmodeot meg nem hivunk, mer az gaz
	glLoadMatrixf(glm::value_ptr(Proj.GetMatrix()));
	glMatrixMode(GL_MODELVIEW);
	if(&Proj==&mWorldProjector)
		glLoadMatrixf(glm::value_ptr(mCamera.GetView()));
	// ez egy faek, a camera osztaly majd csinal egy view matrixot, a model meg setupolja a model matrixot a rendereleskor (food for thought: vagy az legyen mindeig meg minden objra?)
	// vertex=proj*view*model*inVertex majd a shaderben
	else
		glLoadIdentity();

	mShaderManager.UploadGlobalData(GlobalShaderData::WorldProjection,mWorldProjector.GetMatrix());
	mShaderManager.UploadGlobalData(GlobalShaderData::WorldCamera,mCamera.GetView());
	mShaderManager.UploadGlobalData(GlobalShaderData::UiProjection,mUiProjector.GetMatrix());
}

bool Renderer::BeginRender()
{
	glClear(GL_COLOR_BUFFER_BIT);
	return true;
}

bool Renderer::EndRender()
{
	return true;
}

void Renderer::OnMouseMoveEvent( const ScreenMouseMoveEvent& Event )
{
	glm::vec3 EvtPos(Event.Pos.x,Event.Pos.y,0);
	glm::vec3 UiEvtPos=mUiProjector.Unproject(EvtPos);
	UiMouseMoveEvent UiEvt(glm::vec2(UiEvtPos.x,UiEvtPos.y));
	if(EventServer<UiMouseMoveEvent>::Get().SendEvent(UiEvt))return;

	glm::vec3 WorldEvtPos(mCamera.GetInverseView()*glm::vec4(mWorldProjector.Unproject(EvtPos),1.0));
	WorldMouseMoveEvent WorldEvt(glm::vec2(WorldEvtPos.x,WorldEvtPos.y));
	EventServer<WorldMouseMoveEvent>::Get().SendEvent(WorldEvt);
}

void Renderer::OnMousePressEvent( const ScreenMousePressEvent& Event )
{
	glm::vec3 EvtPos(Event.Pos.x,Event.Pos.y,0);
	glm::vec3 UiEvtPos=mUiProjector.Unproject(EvtPos);
	UiMousePressEvent UiEvt(glm::vec2(UiEvtPos.x,UiEvtPos.y),Event.Button);
	if(EventServer<UiMousePressEvent>::Get().SendEvent(UiEvt))return;

	glm::vec3 WorldEvtPos(mCamera.GetInverseView()*glm::vec4(mWorldProjector.Unproject(EvtPos),1.0));
	WorldMousePressEvent WorldEvt(glm::vec2(WorldEvtPos.x,WorldEvtPos.y),Event.Button);
	EventServer<WorldMousePressEvent>::Get().SendEvent(WorldEvt);
}

void Renderer::OnMouseReleaseEvent(const ScreenMouseReleaseEvent& Event)
{
	glm::vec3 EvtPos(Event.Pos.x,Event.Pos.y,0);
	glm::vec3 UiEvtPos=mUiProjector.Unproject(EvtPos);
	UiMouseReleaseEvent UiEvt(glm::vec2(UiEvtPos.x,UiEvtPos.y),Event.Button);
	if(EventServer<UiMouseReleaseEvent>::Get().SendEvent(UiEvt))return;

	glm::vec3 WorldEvtPos(mCamera.GetInverseView()*glm::vec4(mWorldProjector.Unproject(EvtPos),1.0));
	WorldMouseReleaseEvent WorldEvt(glm::vec2(WorldEvtPos.x,WorldEvtPos.y),Event.Button);
	EventServer<WorldMouseReleaseEvent>::Get().SendEvent(WorldEvt);
}

void Renderer::Init()
{
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	mVAO.Init();
}

