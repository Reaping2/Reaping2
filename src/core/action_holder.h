#ifndef INCLUDED_ACTION_HOLDER_H
#define INCLUDED_ACTION_HOLDER_H

class Action;
class ActionHolder : public Singleton<ActionHolder>
{
	friend class Singleton<ActionHolder>;
	ActionHolder();
	~ActionHolder(){}
public:
	const Action * GetAction(int32_t What) const;
	const Action * GetAction(std::string const& What)const;
private:
	typedef boost::ptr_map<int32_t,Action> ActionMap_t;
	ActionMap_t mActions;
};

#endif//INCLUDED_ACTION_HOLDER_H