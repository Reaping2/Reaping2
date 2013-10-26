#include "window.h"
#include "render/i_render.h"
#include "core/i_core.h"
#include "input/i_input.h"

class FrameCounter
{
	uint32_t mFrames;
	double mStart;
public:
	FrameCounter() : mFrames(0),mStart(glfwGetTime()){}
	void Inc()
	{
		if(!(++mFrames%400))
		{
			LOG("FPS: %f\n",mFrames/(glfwGetTime()-mStart));
		}
	}
};

int main()
{
	Window& Wnd=Window::Get();	// ez legyen az elejen

	if(!Wnd.Create(640,480,"Reaping2"))
		return -1;

	Keyboard::Get();
	Mouse& Jerry=Mouse::Get();
	Renderer& Rend=Renderer::Get();
	Scene&  Scen=Scene::Get();
	static const size_t TestCollisionsOhMygod=100;
	// hat ez 1000nel mar diavetites, szoval a mindent-mindennel nem mukodhet, halisten leginkabb playert kell mindennel, meg lovedekeket, ami olyan max parszaz
	// releaseben mondjuk ugy is atom, de lehet h a fordito rajon, h lofaszt se csinal most a fv
	/*for(int i=0;i<TestCollisionsOhMygod;++i)
		Scen.AddActor(new Creep);*/
	Player* Pl=new Player;
	Pl->SetController(std::auto_ptr<Controller>(new PlayerController));
	Scen.AddActor(Pl);
	static const double MaxFrameRate=100.;
	static const double MinFrameTime=1./MaxFrameRate;
	double Prevtime,Curtime;
	Prevtime=Curtime=glfwGetTime();
	FrameCounter Counter;
	while(true)
	{
		Curtime=glfwGetTime();
		double Dt=Curtime-Prevtime;
		if(Dt<MinFrameTime)
		{
			const double SleepTime=MinFrameTime-Dt;
			boost::this_thread::sleep(boost::posix_time::milliseconds(boost::int64_t(SleepTime*1000.)));
			Dt=MinFrameTime;
			Curtime=glfwGetTime();
		}
		Jerry.Update(Dt);
		Scen.Update(Dt);
		if(!Rend.Render()) break;
		if(!Wnd.Run()) break;
		Prevtime=Curtime;
		Counter.Inc();
	}
	return 0;
}