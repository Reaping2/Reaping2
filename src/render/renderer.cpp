#include "i_render.h"

Renderer::Renderer()
: mWorldProjector(-1.0f,1.0f)
, mUiProjector(0.0f,100.0f,Projection::VM_Fixed)
, mCamera(mWorldProjector)
, mUi(Ui::Get())
{
	Font::Get();
	mMouseMoveId=EventServer<ScreenMouseMoveEvent>::Get().Subscribe(boost::bind(&Renderer::OnMouseMoveEvent,this,_1));
	mMousePressId=EventServer<ScreenMousePressEvent>::Get().Subscribe(boost::bind(&Renderer::OnMousePressEvent,this,_1));
	mMouseReleaseId=EventServer<ScreenMouseReleaseEvent>::Get().Subscribe(boost::bind(&Renderer::OnMouseReleaseEvent,this,_1));
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

bool Renderer::Render()
{
	if(!BeginRender()) return false;
	mCamera.Update();

	// render world
	SetupRenderer(mWorldProjector);
	mSceneRenderer.Draw(Scene::Get());
	RenderActors(false);
	RenderActors(true);

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
	if(&Proj==&mWorldProjector)
		glLoadMatrixf(glm::value_ptr(mCamera.GetView()));
	// ez egy faek, a camera osztaly majd csinal egy view matrixot, a model meg setupolja a model matrixot a rendereleskor (food for thought: vagy az legyen mindeig meg minden objra?)
	// vertex=proj*view*model*inVertex majd a shaderben
	else
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

