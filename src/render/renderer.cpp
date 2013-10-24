#include "i_render.h"
#include "main/window.h"

Renderer::Renderer()
: mWidth(0)
, mHeight(0)
, mRatio(1)
, mModelRepo(ModelRepo::Get())
{
	Window::ResizeCallback* Callback(new Window::ResizeCallback(boost::bind(&Renderer::SetDimensions,this,_1,_2)));
	Window::Get().SetResizeCallback(std::auto_ptr<Window::ResizeCallback>(Callback));
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

bool Renderer::BeginRender()
{
	glViewport(0, 0, mWidth, mHeight);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-mRatio, mRatio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	return true;
}

void Renderer::SetDimensions( const uint32_t Width, const uint32_t Height )
{
	mWidth=Width;
	mHeight=Height;
	mRatio=mHeight?(float)mWidth/mHeight:1.f;
}

bool Renderer::EndRender()
{
	return true;
}

