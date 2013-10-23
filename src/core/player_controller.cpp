#include "i_core.h"
#include "input/i_input.h"

PlayerController::PlayerController()
: mCurrentMovement(0)
, mDirty(true)
{
	Keyboard& Keys=Keyboard::Get();
	Keyboard::KeyEventFunctor CB=boost::bind(&PlayerController::KeyDown,this,_1,_2,_3);
	Keys.SetCallback(GLFW_KEY_W,CB);
	Keys.SetCallback(GLFW_KEY_A,CB);
	Keys.SetCallback(GLFW_KEY_S,CB);
	Keys.SetCallback(GLFW_KEY_D,CB);
}

void PlayerController::KeyDown(int Key, int Mods, KeyState::Type Action)
{
	uint32_t OldMovement=mCurrentMovement;
	uint32_t Mod=0;
	if(Key==GLFW_KEY_W)Mod=MF_Up;
	else if(Key==GLFW_KEY_A)Mod=MF_Left;
	else if(Key==GLFW_KEY_S)Mod=MF_Down;
	else if(Key==GLFW_KEY_D)Mod=MF_Right;
	if(Action==KeyState::Down)
		mCurrentMovement|=Mod;
	else
		mCurrentMovement&=~Mod;
	mDirty=mDirty||mCurrentMovement!=OldMovement;
}

void PlayerController::Update( double Seconds )
{
	if(!mActor)return;
	if(!mDirty)return;
	mDirty=false;
	int x=((mCurrentMovement&MF_Left)?-1:0)+((mCurrentMovement&MF_Right)?1:0);
	int y=((mCurrentMovement&MF_Up)?1:0)+((mCurrentMovement&MF_Down)?-1:0);
	mActor->SetSpeed(std::max<double>(std::abs(x),std::abs(y)));
	if(x==0&&y==0)return;
	double Heading=0;
	static const double pi=boost::math::constants::pi<double>();
	if(x==0)
		Heading=(y<0)?-pi/2:pi/2;
	else if(y==0)
		Heading=(x<0)?-pi:0;
	else if(y>0)
		Heading=(x<0)?pi*0.75:pi/4;
	else
		Heading=(x<0)?pi*1.25:pi*1.75;

	mActor->SetHeading(Heading);
}

PlayerController::~PlayerController()
{
	Keyboard& Keys=Keyboard::Get();
	Keys.ClearCallback(GLFW_KEY_W);
	Keys.ClearCallback(GLFW_KEY_A);
	Keys.ClearCallback(GLFW_KEY_S);
	Keys.ClearCallback(GLFW_KEY_D);
}

