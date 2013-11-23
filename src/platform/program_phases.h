#ifndef INCLUDED_PLATFORM_PROGRAM_PHASES_H
#define INCLUDED_PLATFORM_PROGRAM_PHASES_H

namespace ProgramPhase
{
	enum Type{
		Startup,
		Running,
		CloseSignal,
		Shutdown,
	};
}

struct PhaseChangedEvent : public Event
{
	const ProgramPhase::Type CurrentPhase;
	PhaseChangedEvent(ProgramPhase::Type Phase):CurrentPhase(Phase){}
};

struct CycleEvent : public Event
{
	const double CurrentTime;
	CycleEvent(double Time):CurrentTime(Time){}
};

#endif//INCLUDED_PLATFORM_PROGRAM_PHASES_H
