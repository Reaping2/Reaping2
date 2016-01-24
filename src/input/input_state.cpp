#include "input/input_state.h"

namespace engine {
InputState::InputState()
{
    Reset();
}

void InputState::Reset()
{
    mOrientation=0.0;
    mHeading=0.0;
    mShoot=false;
    mShootAlt=false;
    mUseNormalItem=false;
    mReload=false;
    mMoving=false;
    mShowLeaderboard=false;
    mPause=false;
}

bool InputState::operator==(InputState const& other)
{
    return mOrientation==other.mOrientation
        && mHeading==other.mHeading
        && mShoot==other.mShoot
        && mShootAlt==other.mShootAlt
        && mUseNormalItem==other.mUseNormalItem
        && mReload==other.mReload
        && mMoving==other.mMoving
        && mShowLeaderboard==other.mShowLeaderboard
        && mPause==other.mPause;
}

bool InputState::operator!=(InputState const& other)
{
    return !(*this==other);
}

} // namespace engine
