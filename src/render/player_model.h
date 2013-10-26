#ifndef INCLUDED_RENDERER_PLAYER_MODEL_H
#define INCLUDED_RENDERER_PLAYER_MODEL_H

class PlayerModel : public Singleton<PlayerModel>, public Model
{
	friend class Singleton<PlayerModel>;
	PlayerModel();
public:
	virtual void Draw(Actor const& Object)const;
};

#endif//INCLUDED_RENDERER_PLAYER_MODEL_H
