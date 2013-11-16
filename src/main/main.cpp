#include "window.h"
#include "render/i_render.h"
#include "core/i_core.h"
#include "input/i_input.h"

#include <boost/timer.hpp>

namespace{
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
} PerfTimer;

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

Registration PopulateSceneId;
void PopulateScene()
{
	PopulateSceneId.Unregister();
	Scene& Scen=Scene::Get();

	Actor* grass = new Actor("grass");
	grass->AddAction(AutoId("idle"));
	Scen.AddActor(grass);

	Player* Pl=new Player();
	Pl->SetController(std::auto_ptr<Controller>(new PlayerController));
	Scen.AddActor(Pl);
	PerfTimer.Log("scene pop");
}
}

int main()
{
	Window& Wnd=Window::Get();	// ez legyen az elejen
	if(!Wnd.Create(640,480,"Reaping2"))
		return -1;
	PerfTimer.Log("wnd");
	TimerServer& Timers(TimerServer::Get());
	Filesys::Get().Mount(std::auto_ptr<Package>(new Package(AutoFile(new OsFile("data.pkg")))));
	Keyboard::Get();
	ActionRepo::Get();
	Mouse& Jerry=Mouse::Get();
	PerfTimer.Log("input");
	Renderer& Rend=Renderer::Get();
	PerfTimer.Log("renderer");
	Scene& Scen=Scene::Get();
	PerfTimer.Log("scene");
	static const double MaxFrameRate=100.;
	static const double MinFrameTime=1./MaxFrameRate;
	double Prevtime,Curtime;
	Prevtime=Curtime=glfwGetTime();
	FrameCounter Counter;
	PopulateSceneId=Timers.AddTimer(Timer::TimerCallback(&PopulateScene),0.5);
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
		Timers.Update(Dt);
		Jerry.Update(Dt);
		Scen.Update(Dt);
		if(!Rend.Render()) break;
		if(!Wnd.Run()) break;
		Prevtime=Curtime;
		Counter.Inc();
	}
	return 0;
}