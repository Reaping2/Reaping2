#ifndef INCLUDED_PLATFORM_EVENT_H
#define INCLUDED_PLATFORM_EVENT_H

struct Event
{
	virtual ~Event();
	Event();
	bool IsHandled()const;
	void SetHandled()const;
private:
	mutable bool mHandled;
};

template<typename Event_T>
class EventServer : public Singleton< EventServer<Event_T> > , public Registry
{
	BOOST_STATIC_ASSERT_MSG(
		(boost::is_base_of<Event, Event_T>::value),
		"Event_T must be a descendant of Event"
		);
public:
	typedef typename Event_T EventType;
	typedef typename boost::function<void(const EventType&)> EventHandler;
	Registration Subscribe(const EventHandler&);
	bool SendEvent(const EventType& Event);
};

template<typename Event_T>
bool EventServer<Event_T>::SendEvent( const EventType& Event )
{
	const Registrations& Regs=this->mRegistrations;
	for(Registrations::const_iterator i=Regs.begin(),e=Regs.end();i!=e;++i)
		((EventHandler const*)(*i))->operator()(Event);
	return Event.IsHandled();
}

template<typename Event_T>
Registration EventServer<Event_T>::Subscribe( const EventHandler& Handler )
{
	return this->Register(new EventHandler(Handler));
}

#endif//INCLUDED_PLATFORM_EVENT_H
