#include "i_render.h"

Renderer::Renderer()
: mWidth(0)
, mHeight(0)
, mRatio(1)
, mModelRepo(ModelRepo::Get())
{
	mWindowResizeId=EventServer<WindowResizeEvent>::Get().Subscribe(boost::bind(&Renderer::OnWindowResizeEvent,this,_1));
	int w,h;
	Window::Get().GetWindowSize(w,h);
	Resize(w,h);
	TextureRepo::Get();
	SetupRenderer();
}

Renderer::~Renderer()
{

}

bool Renderer::Render()
{
	if(mWidth==0||mHeight==0) return true;
	if(!BeginRender()) return false;

	Scene& Scen=Scene::Get();
	const AllActorInSceneList& Lst=Scen.GetActors();
	for(AllActorInSceneList::const_iterator i=Lst.begin(),e=Lst.end();i!=e;++i)
	{
		const Actor& Object=*i;
		Model const& Model=mModelRepo.GetModel(Object);
		Model.Draw(Object);
	}

	return EndRender();
}

void Renderer::SetupRenderer()
{
	glViewport(0, 0, mWidth, mHeight);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glMatrixMode(GL_PROJECTION);
	// todo: ez nyilvan shader programba kerul atzuzasra, a c kod csak a matrixot ismeri
	// glmatrixmodeot meg nem hivunk, mer az gaz
	glm::mat4 Projection=glm::ortho(-mRatio, mRatio, -1.f, 1.f, 1.f, -1.f);
	glLoadMatrixf((const GLfloat*)&Projection[0]);
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

void Renderer::OnWindowResizeEvent(const WindowResizeEvent& Event)
{
	Resize(Event.Width,Event.Height);
	SetupRenderer();
}
void Renderer::Resize(int Width, int Height)
{
	mWidth=Width;
	mHeight=Height;
	mRatio=mHeight?(float)mWidth/mHeight:1.f;
}

bool Renderer::EndRender()
{
	return true;
}

