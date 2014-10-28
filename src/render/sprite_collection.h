#ifndef INCLUDED_RENDER_SPRITE_COLLECTION_H
#define INCLUDED_RENDER_SPRITE_COLLECTION_H

class RenderableRepo;
class SpriteCollection : public Repository<Sprite>
{
    static Sprite DefaultSprite;
    friend class RenderableRepo;
    SpriteCollection();
    int32_t mId;
public:
    int32_t Id()const;
    bool Load( int32_t TexId, Json::Value& Root );
    void Merge( SpriteCollection& Other );
};

#endif//INCLUDED_RENDER_SPRITE_COLLECTION_H
