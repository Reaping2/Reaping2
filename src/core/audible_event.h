#ifndef INCLUDED_CORE_AUDIBLE_EVENT_H
#define INCLUDED_CORE_AUDIBLE_EVENT_H

struct AudibleEvent : public Event
{
	const int32_t mId;
	AudibleEvent(int32_t Id):mId(Id){}
	AudibleEvent(std::string const& Name):mId(AutoId(Name)){}
};

#endif//INCLUDED_CORE_AUDIBLE_EVENT_H
