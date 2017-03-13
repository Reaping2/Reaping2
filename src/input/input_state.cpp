#include "input/input_state.h"

namespace engine {
InputState::InputState()
{
    mOrientation = 0.0;
    mHeading = 0.0;
    mCursorX = 0.0;
    mCursorY = 0.0;
    Reset();
}

void InputState::Reset()
{
    mShoot = false;
    mShootAlt = false;
    mUseNormalItem = false;
    mReload = false;
    mMoving = false;
    mShowLeaderboard = false;
    mPause = false;
    mActivate = false;
    mSwitchWeapon = false;
    mSwitchNormalItem = false;
}

bool InputState::operator==( InputState const& other ) const
{
    return mOrientation == other.mOrientation
        && mHeading == other.mHeading
        && mShoot == other.mShoot
        && mShootAlt == other.mShootAlt
        && mUseNormalItem == other.mUseNormalItem
        && mReload == other.mReload
        && mMoving == other.mMoving
        && mShowLeaderboard == other.mShowLeaderboard
        && mPause == other.mPause
        && mCursorX == other.mCursorX
        && mCursorY == other.mCursorY
        && mActivate == other.mActivate
        && mSwitchWeapon == other.mSwitchWeapon
        && mSwitchNormalItem == other.mSwitchNormalItem;
}

bool InputState::operator!=( InputState const& other ) const
{
    return !( *this == other );
}

} // namespace engine
