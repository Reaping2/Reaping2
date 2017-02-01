#include "core/worm_head_component.h"

WormHeadComponent::WormHeadComponent()
    : mLength(0)
    , mLengthDecrease(0)
    , mBodyId(-1)
    , mTailId(-1)
    , mSpawnDelay(1.0)
    , mSpawnDelayMax(1.0)
    , mSpawnCount(0)
    , mFirstBuild(true)
    , mBodyParts()
    , mWaitDistance(0.0)
    , mGapPercent(1.0)
{
}

void WormHeadComponent::SetLength( int32_t length )
{
    mLength = length;
}

int32_t WormHeadComponent::GetLength() const
{
    return mLength;
}

void WormHeadComponent::SetLengthDecrease( int32_t lengthDecrease )
{
    mLengthDecrease = lengthDecrease;
}

int32_t WormHeadComponent::GetLengthDecrease() const
{
    return mLengthDecrease;
}

void WormHeadComponent::SetBodyId( int32_t bodyId )
{
    mBodyId = bodyId;
}

int32_t WormHeadComponent::GetBodyId() const
{
    return mBodyId;
}

void WormHeadComponent::SetTailId( int32_t tailId )
{
    mTailId = tailId;
}

int32_t WormHeadComponent::GetTailId() const
{
    return mTailId;
}

void WormHeadComponent::SetSpawnDelay( double spawnDelay )
{
    mSpawnDelay = spawnDelay;
}

double WormHeadComponent::GetSpawnDelay() const
{
    return mSpawnDelay;
}

void WormHeadComponent::SetSpawnDelayMax( double spawnDelayMax )
{
    mSpawnDelayMax = spawnDelayMax;
}

double WormHeadComponent::GetSpawnDelayMax() const
{
    return mSpawnDelayMax;
}

void WormHeadComponent::SetSpawnCount( int32_t spawnCount )
{
    mSpawnCount = spawnCount;
}

int32_t WormHeadComponent::GetSpawnCount() const
{
    return mSpawnCount;
}

void WormHeadComponent::SetFirstBuild( bool firstBuild )
{
    mFirstBuild = firstBuild;
}

bool WormHeadComponent::IsFirstBuild() const
{
    return mFirstBuild;
}

void WormHeadComponent::SetBodyParts( BodyParts_t const& bodyParts )
{
    mBodyParts = bodyParts;
}

IWormHeadComponent::BodyParts_t& WormHeadComponent::GetBodyParts()
{
    return mBodyParts;
}


void WormHeadComponent::SetPrevPositions( PrevPositions_t const& prevPositions )
{
    mPrevPositions = prevPositions;
}

IWormHeadComponent::PrevPositions_t& WormHeadComponent::GetPrevPositions()
{
    return mPrevPositions;
}

void WormHeadComponent::SetWaitDistance( double waitDistance )
{
    mWaitDistance = waitDistance;
}

double WormHeadComponent::GetWaitDistance() const
{
    return mWaitDistance;
}

void WormHeadComponent::SetGapPercent( double gapPercent )
{
    mGapPercent = gapPercent;
}

double WormHeadComponent::GetGapPercent() const
{
    return mGapPercent;
}


void WormHeadComponent::CopyFrom( IWormHeadComponent* iHeadC )
{
    int32_t actorGUID = mActorGUID;
    auto* headC = dynamic_cast<WormHeadComponent*>(iHeadC);
    if (headC)
    {
        *this = *headC;
    }
    mActorGUID = actorGUID;
}

void WormHeadComponentLoader::BindValues()
{
    Bind( "length", func_int32_t(&WormHeadComponent::SetLength));
    Bind( "length_decrease", func_int32_t( &WormHeadComponent::SetLengthDecrease ) );
    std::string istr;
    if (Json::GetStr( (*mSetters)["body"], istr ))
    {
        Bind<int32_t>( &WormHeadComponent::SetBodyId, AutoId( istr ) );
    }
    if (Json::GetStr( (*mSetters)["tail"], istr ))
    {
        Bind<int32_t>( &WormHeadComponent::SetTailId, AutoId( istr ) );
    }
    Bind( "spawn_delay", func_int32_t( &WormHeadComponent::SetSpawnDelayMax ) );
    Bind( "spawn_delay", func_int32_t( &WormHeadComponent::SetSpawnDelay ) );
    Bind( "length", func_int32_t( &WormHeadComponent::SetSpawnCount ) );
    Bind( "gap_percent", func_double( &WormHeadComponent::SetGapPercent ) );
}

WormHeadComponentLoader::WormHeadComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( WormHeadComponent, WormHeadComponent );
