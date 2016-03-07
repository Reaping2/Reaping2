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
public:
    int32_t Id()const;
    bool Load( int32_t TexId, Json::Value& Root );
    void Merge( SpriteCollection& Other );
};

#endif//INCLUDED_RENDER_SPRITE_COLLECTION_H
