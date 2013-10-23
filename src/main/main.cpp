#include "window.h"
#include "render/i_render.h"
#include "core/i_core.h"

int main()
{
	Window& Wnd=Window::Get();	// ez legyen az elejen

	if(!Wnd.Create(640,480,"Reaping2"))
		return -1;

	Renderer& Rend=Renderer::Get();
	Scene&  Scen=Scene::Get();
	static const size_t TestCollisionsOhMygod=100;
	// hat ez 1000nel mar diavetites, szoval a mindent-mindennel nem mukodhet, halisten leginkabb playert kell mindennel, meg lovedekeket, ami olyan max parszaz
	// releaseben mondjuk ugy is atom, de lehet h a fordito rajon, h lofaszt se csinal most a fv
	for(int i=0;i<TestCollisionsOhMygod;++i)
		Scen.AddActor(new Actor);
	double prevtime,curtime;
	prevtime=curtime=glfwGetTime();
	while(true)
	{
		curtime=glfwGetTime();
		Scen.Update(curtime-prevtime);
		if(!Rend.Render()) break;
		if(!Wnd.Run()) break;
		prevtime=curtime;
	}
	return 0;
}