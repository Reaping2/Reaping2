#ifndef INCLUDED_PLATFORM_PROGRAM_PHASES_H
#define INCLUDED_PLATFORM_PROGRAM_PHASES_H

namespace platform {

namespace ProgramPhase {
enum Type
{
    Startup,
    Running,
    InitiateShutdown,
    CloseSignal,
    Shutdown,
};
} // namespace ProgramPhase

struct PhaseChangedEvent : public Event
{
    const ProgramPhase::Type CurrentPhase;
    PhaseChangedEvent( ProgramPhase::Type Phase ): CurrentPhase( Phase ) {}
};

struct CycleEvent : public Event
{
    const double CurrentTime;
    CycleEvent( double Time ): CurrentTime( Time ) {}
};

} // namespace platform

#endif//INCLUDED_PLATFORM_PROGRAM_PHASES_H
