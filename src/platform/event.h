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
        ( boost::is_base_of<Event, Event_T>::value ),
        "Event_T must be a descendant of Event"
    );
    virtual void UpdateOne( void* RegistrationData, void* UpdateData );
    virtual void DeleteData( void* Data );
    static bool mEventServerMustBeInitedFirst;
    friend class Singleton<EventServer<Event_T> >;
    EventServer();
public:
    typedef Event_T EventType;
    typedef typename boost::function<void( const EventType& )> EventHandler;
    Registration Subscribe( const EventHandler& );
    bool SendEvent( const EventType& Event );
};

template<typename Event_T>
EventServer<Event_T>::EventServer()
{
    ( void )mEventServerMustBeInitedFirst;
}

template<typename Event_T>
bool EventServer<Event_T>::mEventServerMustBeInitedFirst = ( EventServer<Event_T>::Get(), true );

template<typename Event_T>
void EventServer<Event_T>::DeleteData( void* Data )
{
    delete ( EventHandler* )Data;
}

template<typename Event_T>
void EventServer<Event_T>::UpdateOne( void* RegistrationData, void* UpdateData )
{
    EventType const& Evt = *( EventType const* )UpdateData;
    EventHandler const* Hndl = ( EventHandler const* )RegistrationData;
    Hndl->operator()( Evt );
}

template<typename Event_T>
bool EventServer<Event_T>::SendEvent( const EventType& Event )
{
    Registry::Update( ( void* )&Event );
    return Event.IsHandled();
}

template<typename Event_T>
Registration EventServer<Event_T>::Subscribe( const EventHandler& Handler )
{
    return this->Register( new EventHandler( Handler ) );
}

#endif//INCLUDED_PLATFORM_EVENT_H
