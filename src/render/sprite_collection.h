#ifndef INCLUDED_RENDER_SPRITE_COLLECTION_H
#define INCLUDED_RENDER_SPRITE_COLLECTION_H
#include "platform/i_platform.h"
#include "sprite.h"

class RenderableRepo;
//class Sprite;
class SpriteCollection : public Repository<Sprite>
{
    Sprite mDefaultSprite;
    friend class RenderableRepo;
    SpriteCollection();
    int32_t mId;
    int32_t mJointId;
public:
    int32_t Id()const;
    int32_t JointId()const;
    bool Load( int32_t TexId, Json::Value const& Root );
    void Merge( SpriteCollection& Other );
};

#endif//INCLUDED_RENDER_SPRITE_COLLECTION_H
