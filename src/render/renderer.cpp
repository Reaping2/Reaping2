#include "i_render.h"
#include "main/window.h"
#include "core/i_core.h"

Renderer::Renderer()
: mWidth(0)
, mHeight(0)
, mRatio(1)
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
	for(int z=0;z<CollisionClass::Num_Classes;++z)
	{
		const ActorList& Lst=Scen.GetActors()[z];	// ez a collisionclassonkenti rajzolas fos
		// nem tuti, h az kell, de fejben latszolag sok hivast sporol
		// persze a teljesitmenyt telibeszarom
		for(ActorList::const_iterator i=Lst.begin(),e=Lst.end();i!=e;++i)
		{
			const Actor& Object=*i;
			// nyilvan itt push matrix/pop matrix, de perpill leszarom azt is
			glTranslatef((GLfloat)Object.GetX(),(GLfloat)Object.GetY(),0);
			glRotatef((GLfloat)Object.GetHeading() * 50.f, 0.f, 0.f, 1.f);
			// meg azt is, hogy ez egy VBO
			glBegin(GL_TRIANGLES);
			glColor3f(1.f, 0.f, 0.f);
			glVertex3f(-0.6f, -0.4f, (GLfloat)z/CollisionClass::Num_Classes);
			glColor3f(0.f, 1.f, 0.f);
			glVertex3f(0.6f, -0.4f, (GLfloat)z/CollisionClass::Num_Classes);
			glColor3f(0.f, 0.f, 1.f);
			glVertex3f(0.f, 0.6f, (GLfloat)z/CollisionClass::Num_Classes);
			glEnd();
			glRotatef((GLfloat)-Object.GetHeading() * 50.f, 0.f, 0.f, 1.f);
			glTranslatef((GLfloat)-Object.GetX(),(GLfloat)-Object.GetY(),0);
		}
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

