#ifndef INCLUDED_ENGINE_INPUT_STATE_H
#define INCLUDED_ENGINE_INPUT_STATE_H

namespace engine {

struct InputState
{
    double mOrientation;
    double mHeading;
    bool mShoot;
    bool mShootAlt;
    bool mUseNormalItem;
    bool mReload;
    bool mMoving;
    bool mShowLeaderboard;
    bool mPause;
    bool mActivate;
    bool mSwitchWeapon;
    bool mSwitchNormalItem;
    double mCursorX;
    double mCursorY;
    InputState();
    void Reset();
    bool operator==( InputState const& other ) const;
    bool operator!=( InputState const& other ) const;
};

} // namespace engine

#endif//INCLUDED_ENGINE_INPUT_STATE_H

//command:  "classgenerator.exe" -g "event" -c "input_state" -n "engine" -m "double-orientation double-heading bool-shoot bool-shootAlt bool-useNormalItem bool-revive bool-reload"
