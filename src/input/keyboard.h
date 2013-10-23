#ifndef INCLUDED_INPUT_KEYBOARD_H
#define INCLUDED_INPUT_KEYBOARD_H

struct KeyState{
	enum Type
	{
		Down,
		Up,
	};
};

class Keyboard : public Singleton<Keyboard>
{
public:
	typedef boost::function<void(int,int,KeyState::Type)> KeyEventFunctor;
	void SetCallback(int Key, const KeyEventFunctor& Functor);
	void ClearCallback(int Key);
	typedef boost::function<void(int)> UniCharFunctor;
	void SetCharCallback(const UniCharFunctor& Functor);
private:
	friend class Singleton<Keyboard>;
	Keyboard();

	void OnKey(int Key, int Scan, int Action, int Mods);

	typedef std::map<int,KeyEventFunctor> KeyEventFunctors;
	KeyEventFunctors mFunctors;	// direkt nem vektor, csak egy handler minden keyre; ha tobb kell, akkor valamit elkurtunk. pl. az iranyitast.
};

#endif//INCLUDED_INPUT_KEYBOARD_H
