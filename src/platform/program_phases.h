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

#endif//INCLUDED_PLATFORM_PROGRAM_PHASES_H
