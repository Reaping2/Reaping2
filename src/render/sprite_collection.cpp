#include "i_render.h"
#include "sprite_collection.h"
#include "sprite.h"

SpriteCollection::SpriteCollection()
    : RepoBase( mDefaultSprite )
    , mDefaultSprite( 0, Json::Value() )
    , mId( 0 )
    , mJointId( -1 )
{

}

int32_t SpriteCollection::Id() const
{
    return mId;
}

int32_t SpriteCollection::JointId() const
{
    return mJointId;
}

bool SpriteCollection::Load( int32_t TexId, Json::Value const& Root )
{
    std::string NameStr;
    if( !Json::GetStr( Root["name"], NameStr ) )
    {
        return false;
    }
    std::string JointCollection;
    if( Json::GetStr( Root["joint_sprite_collection"], JointCollection ) )
    {
        mJointId = AutoId( JointCollection );
    }
    Json::Value Anims = Root["animations"];
    if( !Anims.isArray() )
    {
        return false;
    }
    ElementMap_t Animations;
    for( Json::Value::iterator i = Anims.begin(), e = Anims.end(); i != e; ++i )
    {
        Json::Value& Anim = *i;
        if( !Anim.isObject() )
        {
            return false;
        }
        std::string AnimationName;
        if( !Json::GetStr( Anim["name"], AnimationName ) )
        {
            return false;
        }
        int32_t ActionId = AutoId( AnimationName );
        Sprite* Spr = new Sprite( TexId, Anim );
        Animations.insert( ActionId, Spr );
    }
    // all is good
    mId = AutoId( NameStr );
    mElements.transfer( Animations.begin(), Animations.end(), Animations );
    return true;
}

void SpriteCollection::Merge( SpriteCollection& Other )
{
    assert( mId == Other.mId );
    mElements.transfer( Other.mElements.begin(), Other.mElements.end(), Other.mElements );
}

