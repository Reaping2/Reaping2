#include "i_render.h"

Renderer::Renderer()
: mWorldProjector(-1.0f,1.0f)
, mUiProjector(0.0f,100.0f,Projection::VM_Fixed)
{
	Font::Get();
	mMouseMoveId=EventServer<ScreenMouseMoveEvent>::Get().Subscribe(boost::bind(&Renderer::OnMouseMoveEvent,this,_1));
	mMousePressId=EventServer<ScreenMousePressEvent>::Get().Subscribe(boost::bind(&Renderer::OnMousePressEvent,this,_1));
	mMouseReleaseId=EventServer<ScreenMouseReleaseEvent>::Get().Subscribe(boost::bind(&Renderer::OnMouseReleaseEvent,this,_1));
}

Renderer::~Renderer()
{

}

bool Renderer::Render()
{
	if(!BeginRender()) return false;

	// render world
	SetupRenderer(mWorldProjector);
	Scene& Scen=Scene::Get();
	const AllActorInSceneList& Lst=Scen.GetActors();
	for(AllActorInSceneList::const_iterator i=Lst.begin(),e=Lst.end();i!=e;++i)
	{
		static ModelRepo const& Models(ModelRepo::Get());
		const Actor& Object=*i;
		Model const& Model=Models(Object.GetId());
		Model.Draw(Object);
	}

	// render ui
	SetupRenderer(mUiProjector);

	for(Widget::const_iterator i=mUiRoot.begin(),e=mUiRoot.end();i!=e;++i)
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
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	Viewport const& Vp=Proj.GetViewport();
	glViewport(Vp.X, Vp.Y, Vp.Width, Vp.Height);
	glMatrixMode(GL_PROJECTION);
	// todo: ez nyilvan shader programba kerul atzuzasra, a c kod csak a matrixot ismeri
	// glmatrixmodeot meg nem hivunk, mer az gaz
	glLoadMatrixf(glm::value_ptr(Proj.GetMatrix()));
	glMatrixMode(GL_MODELVIEW);
	// ez egy faek, a camera osztaly majd csinal egy view matrixot, a model meg setupolja a model matrixot a rendereleskor (food for thought: vagy az legyen mindeig meg minden objra?)
	// vertex=proj*view*model*inVertex majd a shaderben
	glLoadIdentity();
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

	glm::vec3 WorldEvtPos=mWorldProjector.Unproject(EvtPos);
	WorldMouseMoveEvent WorldEvt(glm::vec2(WorldEvtPos.x,WorldEvtPos.y));
	EventServer<WorldMouseMoveEvent>::Get().SendEvent(WorldEvt);
}

void Renderer::OnMousePressEvent( const ScreenMousePressEvent& Event )
{
	glm::vec3 EvtPos(Event.Pos.x,Event.Pos.y,0);
	glm::vec3 UiEvtPos=mUiProjector.Unproject(EvtPos);
	UiMousePressEvent UiEvt(glm::vec2(UiEvtPos.x,UiEvtPos.y),Event.Button);
	if(EventServer<UiMousePressEvent>::Get().SendEvent(UiEvt))return;

	//todo: event feliratkozas, aztan v a root singleton, vagy a uimgr hivja
	Widget* Wdg=mUiRoot.GetHit(UiEvt.Pos);
	if(Wdg)
	{
		bool Flagged=!!(int32_t)(*Wdg)(Widget::PT_Flagged);
		(*Wdg)(Widget::PT_Flagged)=Flagged?0:1;
		(*Wdg)(Widget::PT_Color)=Flagged?0x770000:0xff0000;
		return;
	}

	glm::vec3 WorldEvtPos=mWorldProjector.Unproject(EvtPos);
	WorldMousePressEvent WorldEvt(glm::vec2(WorldEvtPos.x,WorldEvtPos.y),Event.Button);
	EventServer<WorldMousePressEvent>::Get().SendEvent(WorldEvt);
}

void Renderer::OnMouseReleaseEvent(const ScreenMouseReleaseEvent& Event)
{
	glm::vec3 EvtPos(Event.Pos.x,Event.Pos.y,0);
	glm::vec3 WorldEvtPos=mWorldProjector.Unproject(EvtPos);
	WorldMouseReleaseEvent WorldEvt(glm::vec2(WorldEvtPos.x,WorldEvtPos.y),Event.Button);
	EventServer<WorldMouseReleaseEvent>::Get().SendEvent(WorldEvt);
}

