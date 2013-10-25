#ifndef INCLUDED_PLATFORM_EVENT_H
#define INCLUDED_PLATFORM_EVENT_H

template<typename Event_T>
class EventServer : public Singleton< EventServer<Event_T> > , public Registry
{
public:
	typedef typename Event_T EventType;
	typedef typename boost::function<void(const EventType&)> EventHandler;
	Registration Subscribe(const EventHandler&);
	void SendEvent(const EventType& Event);
};

template<typename Event_T>
void EventServer<Event_T>::SendEvent( const EventType& Event )
{
	const Registrations& Regs=this->mRegistrations;
	for(Registrations::const_iterator i=Regs.begin(),e=Regs.end();i!=e;++i)
		((EventHandler const*)(*i))->operator()(Event);
}

template<typename Event_T>
Registration EventServer<Event_T>::Subscribe( const EventHandler& Handler )
{
	return this->Register(new EventHandler(Handler));
}

#endif//INCLUDED_PLATFORM_EVENT_H
