#ifndef INCLUDED_PLATFORM_REGISTER_H
#define INCLUDED_PLATFORM_REGISTER_H

class Registry;
class Registration
{
	friend class Registry;
	Registry* mRegister;
	void* mData;
	Registration(Registry* Parent, void* Data);
public:
	~Registration();
	Registration();
	Registration(Registration& O);
	Registration& operator=(Registration& O);
	void const* GetData()const{return mData;}
	void Unregister();
};

class Registry
{
	friend class Registration;
	void Unregister(Registration* Reg);
protected:
	typedef std::vector<void*> Registrations;
	Registrations mRegistrations;
	Registration Register(void *Data);
};

#endif//INCLUDED_PLATFORM_REGISTER_H
