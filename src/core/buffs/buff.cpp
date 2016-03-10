#include "platform/i_platform.h"
#include "core/buffs/buff.h"

int32_t Buff::mNextUId = 0;

Buff::Buff()
    : mSecsToEnd( 0.0 )
    , mAutoRemove( true )
    , mUID( mNextUId++ )
{
}

double Buff::GetSecsToEnd()const
{
    return mSecsToEnd;
}

void Buff::SetSecsToEnd( double secsToEnd )
{
    mSecsToEnd = secsToEnd;
}

void Buff::SetAutoRemove( bool autoRemove )
{
    mAutoRemove = autoRemove;
}

bool Buff::IsAutoRemove()const
{
    return mAutoRemove;
}

void Buff::SetUID( int32_t uID )
{
    mUID = uID;
}

int32_t Buff::GetUID()const
{
    return mUID;
}

Buff::~Buff()
{
}

DefaultBuff::DefaultBuff()
    : Buff()
{
}

Buff::SecsToEndModifier::SecsToEndModifier( double secsToEnd )
    : mSecsToEnd( secsToEnd )
{

}

void Buff::SecsToEndModifier::operator()( Opt<Buff>& Obj )
{
    Obj->SetSecsToEnd(mSecsToEnd);
}
