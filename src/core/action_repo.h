#ifndef INCLUDED_ACTION_HOLDER_H
#define INCLUDED_ACTION_HOLDER_H

class Action;
class Actor;
class ActionRepo :public Singleton<ActionRepo>
{
public:
	ActionRepo();
private:
	Factory<Action, Actor>& mFactory;
	friend class Singleton<ActionRepo>;
};


#endif//INCLUDED_ACTION_HOLDER_H