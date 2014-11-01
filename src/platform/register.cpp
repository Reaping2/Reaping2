#include "register.h"
#include <algorithm>
#include <boost/assert.hpp>

namespace platform {

Registration::~Registration()
{
    //  Unregister();
}

Registration::Registration( Registration const& O ) : mData( NULL ), mRegister( NULL )
{
    *this = O;
}

Registration::Registration( Registry* Parent, void* Data )
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
        //using std::swap;
        mData = O.mData;
        mRegister = O.mRegister;
    }
    return *this;
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

Registration Registry::Register( void* Data )
{
    Registration Reg( this, Data );
    mRegistrations.push_back( Data );
    return Reg;
}

void Registry::Unregister( Registration* Reg )
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
            BOOST_ASSERT( !mErasedDuringUpdate );
            mErasedDuringUpdate = Reg->mData;
        }
    }
    while( false );
    DeleteData( Reg->mData );
    Reg->mData = NULL;
    Reg->mRegister = NULL;
}

void Registry::Update( void* UpdateData )
{
    BOOST_ASSERT( mUpdatedRegistrations.empty() );
    while( !mRegistrations.empty() )
    {
        mErasedDuringUpdate = NULL;
        void* RegistrationData = mRegistrations.front();
        mRegistrations.pop_front();
        UpdateOne( RegistrationData, UpdateData );
        if( mErasedDuringUpdate != RegistrationData )
        {
            mUpdatedRegistrations.push_back( RegistrationData );
        }
    }
    using std::swap;
    swap( mRegistrations, mUpdatedRegistrations );
    BOOST_ASSERT( mUpdatedRegistrations.empty() );
}

Registry::~Registry()
{
    // static dtor kb random ordere miatt ez meg kemenyen be fog jonni.
    // el ko viselni, meg idovel javitani
    // eddig is fos volt, csak most mar van ra assert
    BOOST_ASSERT( mRegistrations.empty() );
}

Registry::Registry()
    : mErasedDuringUpdate( NULL )
{

}

} // namespace platform

