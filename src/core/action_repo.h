#ifndef INCLUDED_ACTION_HOLDER_H
#define INCLUDED_ACTION_HOLDER_H

class Action;
class ActionRepo : public Repository<ActionRepo,Action>
{
	friend class Singleton<ActionRepo>;
	ActionRepo();
	~ActionRepo(){}
};

#endif//INCLUDED_ACTION_HOLDER_H