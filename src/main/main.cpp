#include "window.h"
#include "render/i_render.h"
#include "core/i_core.h"
#include "input/i_input.h"
#include "audio/i_audio.h"

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
		L1("Timer - %s: %f %f\n",Str.c_str(),Now,Now-mPrevMeasurement);
		mPrevMeasurement=Now;
	}
	Timer_t():mMeasurer(),mPrevMeasurement(mMeasurer.elapsed()){Log("timer init");}
} PerfTimer;

class FrameCounter
{
	uint32_t mFrames;
	double mStart;
	double mPrev;
public:
	FrameCounter() : mFrames(0),mStart(glfwGetTime()),mPrev(mStart){}
	void Inc()
	{
		++mFrames;
		double const Now=glfwGetTime();
		double const Diff=Now-mPrev;
		if(Diff>=2.0)
		{
			L1("FPS: %f\n",mFrames/Diff);
			mPrev=Now;
			mFrames=0;
		}
	}
};
}

int main()
{
	Window& Wnd=Window::Get();	// ez legyen az elejen
	if(!Wnd.Create(640,480,"Reaping2"))
		return -1;
	RootModel::Get();
	EventServer<PhaseChangedEvent>& PhaseChangeEventServer(EventServer<PhaseChangedEvent>::Get());
	PhaseChangeEventServer.SendEvent(PhaseChangedEvent(ProgramPhase::Startup));
	PerfTimer.Log("wnd");
	TimerServer& Timers(TimerServer::Get());
	Filesys::Get().Mount(std::auto_ptr<Package>(new Package(AutoFile(new OsFile("data.pkg")))));
	Keyboard::Get();
	AudioEffectPlayer::Get();
	AudioPlayer::Get().Play("sounds/Zap_Beat.ogg",AudioFile::Music);
	Mouse& Jerry=Mouse::Get();
	PerfTimer.Log("input");
	Renderer& Rend=Renderer::Get();
	DamageDecals::Get();
	PerfTimer.Log("renderer");
	Scene& Scen=Scene::Get();
	PerfTimer.Log("scene");
	static const double MaxFrameRate=100.;
	static const double MinFrameTime=1./MaxFrameRate;
	double Prevtime,Curtime;
	Prevtime=Curtime=glfwGetTime();
	FrameCounter Counter;
	PhaseChangeEventServer.SendEvent(PhaseChangedEvent(ProgramPhase::Running));
	EventServer<CycleEvent>& CycleEventServer(EventServer<CycleEvent>::Get());
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
		CycleEventServer.SendEvent(CycleEvent(Curtime));
		if(!Rend.Render()) break;
		if(!Wnd.Run()) break;
		Prevtime=Curtime;
		Counter.Inc();
	}
	PhaseChangeEventServer.SendEvent(PhaseChangedEvent(ProgramPhase::CloseSignal));
	PhaseChangeEventServer.SendEvent(PhaseChangedEvent(ProgramPhase::Shutdown));
	return 0;
}