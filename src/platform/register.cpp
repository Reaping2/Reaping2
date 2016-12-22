#include "register.h"
#include <list>
#include <algorithm>
#include <boost/assert.hpp>

namespace platform {
namespace reg_detail {

class RegistryImpl
{
    Registry& mOwner;
    typedef std::list<void*> Registrations;
    Registrations mRegistrations;

    bool mCurrentRegDeleted;
    Registrations mUpdatedRegistrations;

public:
    void Unregister( Registration* Reg );
    Registration Register( void* Data );
    void Update( void* UpdateData );
    RegistryImpl( Registry& owner );
    ~RegistryImpl();
};

Registration RegistryImpl::Register( void* Data )
{
    Registration Reg( this, Data );
    mRegistrations.push_back( Data );
    return Reg;
}

void RegistryImpl::Unregister( Registration* Reg )
{
    do
    {
        Registrations::iterator it = std::find( mRegistrations.begin(), mRegistrations.end(), Reg->mData );
        if( it != mRegistrations.end() )
        {
            mRegistrations.erase( it );
            break;
        }
        it = std::find( mUpdatedRegistrations.begin(), mUpdatedRegistrations.end(), Reg->mData );
        if( it != mUpdatedRegistrations.end() )
        {
            mUpdatedRegistrations.erase( it );
            break;
        }
        else
        {
            BOOST_ASSERT( !mCurrentRegDeleted );
            mCurrentRegDeleted = true;
        }
    }
    while( false );
    mOwner.DeleteData( Reg->mData );
    Reg->mData = NULL;
    Reg->mRegister = NULL;
}

void RegistryImpl::Update( void* UpdateData )
{
    BOOST_ASSERT( mUpdatedRegistrations.empty() );
    while( !mRegistrations.empty() )
    {
        mCurrentRegDeleted = false;
        void* RegistrationData = mRegistrations.front();
        mRegistrations.pop_front();
        mOwner.UpdateOne( RegistrationData, UpdateData );
        if( !mCurrentRegDeleted )
        {
            mUpdatedRegistrations.push_back( RegistrationData );
        }
    }
    using std::swap;
    swap( mRegistrations, mUpdatedRegistrations );
    BOOST_ASSERT( mUpdatedRegistrations.empty() );
}

RegistryImpl::~RegistryImpl()
{
    // static dtor kb random ordere miatt ez meg kemenyen be fog jonni.
    // el ko viselni, meg idovel javitani
    // eddig is fos volt, csak most mar van ra assert
    BOOST_ASSERT( mRegistrations.empty() );
}

RegistryImpl::RegistryImpl( Registry& owner )
    : mOwner( owner )
    , mCurrentRegDeleted( false )
{

}

} // namespace detail

Registration::Registration( Registration const& O )
    : mRegister( NULL )
    , mData( NULL )
{
    *this = O;
}

Registration::Registration( reg_detail::RegistryImpl* Parent, void* Data )
    : mRegister( Parent )
    , mData( Data )
{

}

Registration::Registration()
    : mRegister( NULL )
    , mData( NULL )
{

}

Registration& Registration::operator=( Registration const& O )
{
    if( this != &O )
    {
        mData = O.mData;
        mRegister = O.mRegister;
    }
    return *this;
}

void const* Registration::GetData() const
{
    return mData;
}

void Registration::Unregister()
{
    if( mRegister )
    {
        mRegister->Unregister( this );
    }
    mRegister = NULL;
    mData = NULL;
}

Registry::Registry()
{
    mImpl.reset( new reg_detail::RegistryImpl( *this ) );
}

Registry::~Registry()
{
}

Registration Registry::Register( void* Data )
{
    return mImpl->Register( Data );
}

void Registry::Update( void* Data )
{
    mImpl->Update( Data );
}

AutoReg::AutoReg()
{
}

AutoReg::AutoReg( Registration const& o )
{
    mRegistration = o;
}

AutoReg::~AutoReg()
{
    mRegistration.Unregister();
}

AutoReg& AutoReg::operator=( Registration const& o )
{
    mRegistration = o;
    return *this;
}

} // namespace platform

