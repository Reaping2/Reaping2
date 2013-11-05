#include "i_render.h"

Renderer::Renderer()
: mModelRepo(ModelRepo::Get())
, mWorldProjector(-1.0f,1.0f)
, mUiProjector(0.0f,100.0f,Projection::VM_Fixed)
{
	TextureRepo::Get();
	mMouseMoveId=EventServer<ScreenMouseMoveEvent>::Get().Subscribe(boost::bind(&Renderer::OnMouseMoveEvent,this,_1));
	mMousePressId=EventServer<ScreenMousePressEvent>::Get().Subscribe(boost::bind(&Renderer::OnMousePressEvent,this,_1));
	bool TestUi=false;
	if(TestUi)
		InitTestUi();
}

void Renderer::InitTestUi()
{
	const int I=5;
	const int J=2;
	const int K=2;
	int Id=0;
	for(int i=0;i<I*I;++i)
	{
		Widget* Itr=new Widget;
		TestRootUi.AddChild(Itr);
		for(int j=0;j<J*J;++j)
		{
			Widget* It2=new Widget;
			Itr->AddChild(It2);
			for(int k=0;k<K;++k)
			{
				Widget* It3=new Widget;
				float x=100.f/I*(i/5)+100.f/I/J*(j/2)+100.f/I/J/K*k;
				float y=100.f/I*(i%5)+100.f/I/J*(j%2)+100.f/I/J/K*k;
				It3->SetDimensions(glm::vec4(x,y,x+100./I/J/K,y+100./I/J/K));
				It3->SetVisible(true);
				It2->AddChild(It3);
			}
		}
	}
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
	for(Widget::const_iterator i=TestRootUi.begin(),e=TestRootUi.end();i!=e;++i)
	{
		glm::vec4 const& Wgt=i->GetDimensions();
		GLfloat a=0.25f+(i->IsFlagged()?0.5f:0.f);
		if(!i->IsVisible()) continue;
		glColor4f(.7f, 0.7f, 0.7f,a);	glVertex3f(Wgt.x,Wgt.y,0);
		glColor4f(.7f, 0.7f, 0.7f,a);	glVertex3f(Wgt.x,Wgt.w,0);
		glColor4f(0.f, 0.7f, 0.7f,a);	glVertex3f(Wgt.z,Wgt.w,0);
		glColor4f(0.f, 0.7f, 0.7f,a);	glVertex3f(Wgt.z,Wgt.y,0);
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

	Widget* Wdg=TestRootUi.GetHit(UiEvt.Pos);
	if(Wdg)
	{
		Wdg->ToggleFlag();
		return;
	}

	glm::vec3 WorldEvtPos=mWorldProjector.Unproject(EvtPos);
	WorldMousePressEvent WorldEvt(glm::vec2(WorldEvtPos.x,WorldEvtPos.y),Event.Button);
	EventServer<WorldMousePressEvent>::Get().SendEvent(WorldEvt);
}

