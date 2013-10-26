#ifndef INCLUDED_CORE_PLAYER_CONTROLLER_H
#define INCLUDED_CORE_PLAYER_CONTROLLER_H

#include "input/i_input.h"

class PlayerController : public Controller
{
	void OnKeyEvent(const KeyEvent& Event);
	void OnMouseMoveEvent(const MouseMoveEvent& Event);
	void OnMousePressEvent(const MousePressEvent& Event);
	void UpdateRotation();
	enum MoveFlags{
		MF_Up		=1<<0,
		MF_Down		=1<<1,
		MF_Left		=1<<2,
		MF_Right	=1<<3,
	};
	uint32_t mCurrentMovement;
	double mX;
	double mY;
	bool mDirty;
	Registration mKeyId;
	Registration mMouseMoveId;
	Registration mMousePressId;
public:
	PlayerController();
	~PlayerController();
	virtual void Update(double Seconds);
};

#endif//INCLUDED_CORE_PLAYER_CONTROLLER_H
