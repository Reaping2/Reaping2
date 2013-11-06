#include "i_render.h"

Renderer::Renderer()
: mModelRepo(ModelRepo::Get())
, mWorldProjector(-1.0f,1.0f)
, mUiProjector(0.0f,100.0f,Projection::VM_Fixed)
{
	TextureRepo::Get();
	mMouseMoveId=EventServer<ScreenMouseMoveEvent>::Get().Subscribe(boost::bind(&Renderer::OnMouseMoveEvent,this,_1));
	mMousePressId=EventServer<ScreenMousePressEvent>::Get().Subscribe(boost::bind(&Renderer::OnMousePressEvent,this,_1));
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
		const Actor& Object=*i;
		Model const& Model=mModelRepo.GetModel(Object);
		Model.Draw(Object);
	}

	// render ui
	SetupRenderer(mUiProjector);

	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_BLEND);
	glBegin(GL_QUADS);
	for(Widget::const_iterator i=mUiRoot.begin(),e=mUiRoot.end();i!=e;++i)
	{
		glm::vec4 const& Wgt=i->GetDimensions();
		GLfloat a=0.25f+(i->IsFlagged()?0.5f:0.f);
		if(!i->IsVisible()) continue;
		glColor4f(.7f, 0.7f, 0.7f,a);	glVertex3f(Wgt.x,Wgt.y,0);
		glColor4f(.7f, 0.7f, 0.7f,a);	glVertex3f(Wgt.x,Wgt.y+Wgt.w,0);
		glColor4f(0.f, 0.7f, 0.7f,a);	glVertex3f(Wgt.x+Wgt.z,Wgt.y+Wgt.w,0);
		glColor4f(0.f, 0.7f, 0.7f,a);	glVertex3f(Wgt.x+Wgt.z,Wgt.y,0);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);


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
		Wdg->ToggleFlag();
		return;
	}

	glm::vec3 WorldEvtPos=mWorldProjector.Unproject(EvtPos);
	WorldMousePressEvent WorldEvt(glm::vec2(WorldEvtPos.x,WorldEvtPos.y),Event.Button);
	EventServer<WorldMousePressEvent>::Get().SendEvent(WorldEvt);
}

