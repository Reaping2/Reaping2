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
	Registration(Registration const& O);
	Registration& operator=(Registration const& O);
	void const* GetData()const{return mData;}
	void Unregister();
};

class AutoReg : public Registration
{
public:
	~AutoReg() {Unregister();}
	AutoReg& operator=(Registration const& O){Registration::operator =(O);return *this;}
};

class Registry
{
	friend class Registration;
	void Unregister(Registration* Reg);
protected:
	Registration Register(void *Data);
	virtual void DeleteData(void* Data)=0;
	void Update(void * UpdateData);
	virtual void UpdateOne(void* RegistrationData, void* UpdateData)=0;
private:
	typedef std::list<void*> Registrations;
	Registrations mRegistrations;

	void* mErasedDuringUpdate;
	Registrations mUpdatedRegistrations;
public:
	Registry();
	virtual ~Registry();
};

#endif//INCLUDED_PLATFORM_REGISTER_H
