#ifndef INCLUDED_ACTION_HOLDER_H
#define INCLUDED_ACTION_HOLDER_H

class Action;
class ActionRepo : public Repository<Action>,  public Singleton<ActionRepo>
{
	friend class Singleton<ActionRepo>;
	ActionRepo();
	~ActionRepo(){}
};

#endif//INCLUDED_ACTION_HOLDER_H