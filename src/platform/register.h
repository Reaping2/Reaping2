#ifndef INCLUDED_PLATFORM_REGISTER_H
#define INCLUDED_PLATFORM_REGISTER_H

#include <memory>

namespace platform {

namespace reg_detail {
class RegistryImpl;
}

class Registration
{
    friend class reg_detail::RegistryImpl;
    reg_detail::RegistryImpl* mRegister;
    void* mData;
    Registration( reg_detail::RegistryImpl* Parent, void* Data );
public:
    Registration();
    Registration( Registration const& O );
    Registration& operator=( Registration const& O );
    void const* GetData() const;
    void Unregister();
};

class AutoReg
{
    Registration mRegistration;
    AutoReg( AutoReg const& o );
public:
    AutoReg();
    ~AutoReg();
    AutoReg& operator=( Registration const& O );
};

class Registry
{
private:
    friend class reg_detail::RegistryImpl;
    std::auto_ptr< reg_detail::RegistryImpl > mImpl;
protected:
    Registration Register( void* RegistrationData );
    void Update( void* UpdateData );

    Registry();
    virtual ~Registry();
protected:
    virtual void DeleteData( void* RegistrationData ) = 0;
    virtual void UpdateOne( void* RegistrationData, void* UpdateData ) = 0;
};

} // namespace platform

#endif//INCLUDED_PLATFORM_REGISTER_H
