#include "window.h"
#include "render/i_render.h"
#include "core/i_core.h"
#include "input/i_input.h"

#include <boost/timer.hpp>

class Timer_t
{
	boost::timer mMeasurer;
	double mPrevMeasurement;
public:
	void Log(std::string const& Str=std::string())
	{
		double Now=mMeasurer.elapsed();
		LOG("Timer - %s: %f %f\n",Str.c_str(),Now,Now-mPrevMeasurement);
		mPrevMeasurement=Now;
	}
	Timer_t():mMeasurer(),mPrevMeasurement(mMeasurer.elapsed()){Log("timer init");}
} Timer;

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
	Timer.Log("wnd");
	Filesys::Get().Mount(std::auto_ptr<Package>(new Package(AutoFile(new OsFile("data.pkg")))));
	Keyboard::Get();
	Mouse& Jerry=Mouse::Get();
	Timer.Log("input");
	Renderer& Rend=Renderer::Get();
	Timer.Log("renderer");
	// itt loading screen fog latszani, ha a renderer/ui epp jol van felsetupolva
	Rend.Render();
	Wnd.Run();
	Scene& Scen=Scene::Get();
	Timer.Log("scene");
	Actor* grass = new Actor("grass");
	ActionRepo::Get()(AutoId("idle")).Activate(*grass);
	Scen.AddActor(grass);

	Player* Pl=new Player();
	Pl->SetController(std::auto_ptr<Controller>(new PlayerController));
	Scen.AddActor(Pl);
	Timer.Log("scene pop");
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