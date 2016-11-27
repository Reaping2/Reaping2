#include "i_render.h"
#include "actor_renderer.h"
#include "core/i_position_component.h"
#include "core/i_inventory_component.h"
#include "core/i_health_component.h"
#include "core/i_collision_component.h"
#include "core/i_renderable_component.h"
#include "core/actor.h"
#include "recognizer.h"
#include "shader_manager.h"
#include "input/mouse.h"
#include "engine/engine.h"
#include "core/program_state.h"
#include "core/scene.h"
#include "renderable_repo.h"
#include "i_visual_box_multiplier_component.h"
#include "sprite_phase_cache.h"
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/ref.hpp>

void ActorRenderer::Init()
{
    mVAO.Init();
    mOnActorEvent = EventServer<ActorEvent>::Get().Subscribe( boost::bind( &ActorRenderer::OnActorEvent, this, _1 ) );
    mMouseMoveId = EventServer<WorldMouseMoveEvent>::Get().Subscribe( boost::bind( &ActorRenderer::OnMouseMoveEvent, this, _1 ) );
}

void ActorRenderer::OnActorEvent( ActorEvent const& Evt )
{
    if( Evt.mState == ActorEvent::Added )
    {
        mActionRenderersMap[Evt.mActor->GetGUID()];
    }
    else if ( Evt.mState == ActorEvent::Removed )
    {
        mActionRenderersMap.erase( Evt.mActor->GetGUID() );
    }
}

ActorRenderer::ActorRenderer()
    : mRecognizerRepo( RecognizerRepo::Get() )
    , mActionRendererFactory( ActionRendererFactory::Get() )
    , mPrevSize( 0 )
    , mX(0)
    , mY(0)
    , mTexIndex(0)
    , mPosIndex(0)
    , mHeadingIndex(0)
    , mSizeIndex(0)
    , mColorIndex(0)
{
    Init();
}

namespace {
typedef std::vector<glm::vec2> Positions_t;
typedef std::vector<GLfloat> Floats_t;
typedef std::vector<glm::vec4> TexCoords_t;
typedef std::vector<glm::vec4> Colors_t;
typedef ActorRenderer::RenderableSprites_t RenderableSprites_t;
glm::vec2 visMultiplier( Actor const& actor )
{
    Opt<render::IVisualBoxMultiplierComponent> const& vbox = actor.Get<render::IVisualBoxMultiplierComponent>();
    if( vbox.IsValid() )
    {
        return glm::vec2( vbox->GetWidth(), vbox->GetHeight() );
    }
    return glm::vec2( 1.0, 1.0 );
}
bool isVisible( Actor const& actor, Camera const& camera )
{
    Opt<IPositionComponent> const positionC = actor.Get<IPositionComponent>();
    if( !positionC.IsValid() )
    {
        return false;
    }
    static std::map<int32_t, float> scaleMap;
    auto it = scaleMap.find( actor.GetId() );
    if( scaleMap.end() == it )
    {
        float& f = scaleMap[ actor.GetId() ];
        static RenderableRepo& renderables( RenderableRepo::Get() );
        SpriteCollection const& Sprites = renderables( actor.GetId() );
        for( auto i = Sprites.begin(), e = Sprites.end(); i != e; ++i )
        {
            if( i->second->GetScale() > f )
            {
                f = i->second->GetScale();
            }
        }
        it = scaleMap.find( actor.GetId() );
    }
    float scale = it->second;
    Opt<ICollisionComponent> const collisionC = actor.Get<ICollisionComponent>();
    glm::vec2 const& visMulti = visMultiplier( actor );
    float vmult = std::max<float>( visMulti.x, visMulti.y );
    // 2.0 multiplier: safety
    float size = ( collisionC.IsValid() ? collisionC->GetRadius() : 50 ) * scale * 2.0 * vmult;
    glm::vec4 const& region = camera.VisibleRegion();
    return region.x < positionC->GetX() + size && region.z > positionC->GetX() - size
        && region.y < positionC->GetY() + size && region.w > positionC->GetY() - size;
}
bool getNextTextId( RenderableSprites_t::const_iterator& i, RenderableSprites_t::const_iterator e,
                    glm::vec2*& Positions, GLfloat*& Headings, glm::vec4*& TexCoords, glm::vec2*& Sizes, glm::vec4*& Colors,
                    GLuint& TexId )
{
    if( i == e )
    {
        return false;
    }
    TexId = i->Spr->TexId;
    (*Positions++) = glm::vec2( i->PositionC->GetX(), i->PositionC->GetY() ) + i->RelativePosition;
    (*Headings++) = ( GLfloat )i->PositionC->GetOrientation();

    float const radius = ( i->CollisionC != nullptr ? i->CollisionC->GetRadius() : 50 )*i->Anim->GetScale();
    auto const size = radius * visMultiplier( *(i->Obj) );
    (*Sizes) = size;
    ++Sizes;

    (*TexCoords++) = glm::vec4( i->Spr->Left, i->Spr->Right, i->Spr->Bottom, i->Spr->Top );
    (*Colors++) = i->Color;
    {   // move to cache
        static render::SpritePhaseCache& cache( render::SpritePhaseCache::Get() );
        cache.Request( *i->Spr, std::max( size.x, size.y ) );
    }
    ++i;
    return true;
}
}

void ActorRenderer::Prepare( Scene const& Object, Camera const& camera, double DeltaTime )
{
    ActorList_t const& Lst = Object.GetActors();
    if( Lst.empty() )
    {
        mRenderableSprites.clear(); // renderable sprites still can contain obsolete sprites, so render nothing instead of invalid object
        mCounts.clear();
        return;
    }
    RenderableSprites_t RenderableSprites;
    RenderableSprites.reserve( mRenderableSprites.size() );
    //the template version works well with '=' i just dont know is it really needed, maybe this one is more self explaining
    ActorListFilter<Scene::RenderableActors> wrp( Lst ); //=Object.GetActors<Scene::RenderableComponents>();
    for( ActorListFilter<Scene::RenderableActors>::const_iterator i = wrp.begin(), e = wrp.end(); i != e; ++i )
    {
        const Actor& Object = **i;
        if( !isVisible( Object, camera ) )
        {
            continue;
        }
        auto recogptr = mRecognizerRepo.GetRecognizers( Object.GetId() );
        if( nullptr == recogptr )
        {
            continue;
        }
        Opt<IRenderableComponent> renderableC( Object.Get<IRenderableComponent>() );
        auto const& recognizers = *recogptr;
        RecognizerRepo::ExcludedRecognizers_t excluded;

        ActionRenderersMap_t::iterator actionRenderersIt = mActionRenderersMap.find( Object.GetGUID() );
        BOOST_ASSERT( actionRenderersIt != mActionRenderersMap.end() );
        ActionRenderers_t& actionRenderers = actionRenderersIt->second;

        for ( auto recogIt = recognizers.begin(), recogE = recognizers.end(); recogIt != recogE; ++recogIt )
        {
            auto const& recognizer = *recogIt;
            if ( excluded.find( recognizer.GetId() ) == excluded.end()
                 && recognizer.Recognize( Object ) )
            {
                int32_t actionRendererId = recognizer.GetActionRenderer();
                ActionRenderers_t::iterator foundActionRendererIt =
                    std::find_if( actionRenderers.begin(), actionRenderers.end(), FindActionRenderer( actionRendererId ) );
                if ( foundActionRendererIt == actionRenderers.end() )
                {
                    std::auto_ptr<ActionRenderer> actionRenderer( mActionRendererFactory( actionRendererId ) );
                    actionRenderer->SetOrder( recognizer.GetOrder() );
                    actionRenderer->Init( Object );
                    actionRenderers.insert( actionRenderer );
                }
                auto excludedRecognizers = mRecognizerRepo.GetExcludedRecognizers( recognizer.GetId() );
                if( nullptr != excludedRecognizers )
                {
                    excluded.insert( excludedRecognizers->begin(), excludedRecognizers->end() );
                }
            }
        }
        for ( auto excludedIt = excluded.begin(), excludedE = excluded.end(); excludedIt != excludedE; ++excludedIt )
        {
            auto arIt = std::find_if( actionRenderers.begin(), actionRenderers.end(), FindActionRenderer( *excludedIt ) );
            if( arIt != actionRenderers.end() )
            {
                actionRenderers.erase( arIt );

            }
        }
        for ( auto actionRendererIt = actionRenderers.begin(), actionRendererE = actionRenderers.end(); actionRendererIt != actionRendererE; ++actionRendererIt )
        {
            ActionRenderer& actionRenderer = *actionRendererIt;
            actionRenderer.FillRenderableSprites( Object, *renderableC.Get(), RenderableSprites );
            actionRenderer.Update( DeltaTime );
        }
    }

    std::swap( RenderableSprites, mRenderableSprites );
    size_t CurSize = mRenderableSprites.size();
    if( CurSize == 0 )
    {
        return;
    }

    Positions_t Positions( CurSize );
    Floats_t Headings( CurSize );
    Positions_t Sizes( CurSize );
    TexCoords_t TexCoords( CurSize );
    Colors_t Colors( CurSize );

    glm::vec2* posptr = &Positions[0];
    GLfloat* hptr = &Headings[0];
    glm::vec2* sptr = &Sizes[0];
    glm::vec4* tptr = &TexCoords[0];
    glm::vec4* cptr = &Colors[0];
    RenderableSprites_t::const_iterator i = mRenderableSprites.begin();
    mCounts = render::count(
        boost::lambda::bind( &getNextTextId, boost::ref( i ), mRenderableSprites.end(),
        boost::ref( posptr ), boost::ref( hptr ), boost::ref( tptr ), boost::ref( sptr ), boost::ref( cptr ),
        boost::lambda::_1 )
    );
    mVAO.Bind();

    if( CurSize > mPrevSize )
    {
        mPrevSize = CurSize;
        size_t TotalSize = CurSize * ( sizeof( glm::vec4 ) + 2 * sizeof( glm::vec2 ) + sizeof( GLfloat ) + sizeof( glm::vec4 ) );
        glBufferData( GL_ARRAY_BUFFER, TotalSize, NULL, GL_DYNAMIC_DRAW );
    }

    size_t CurrentOffset = 0;
    size_t CurrentSize = CurSize * sizeof( glm::vec4 );
    GLuint CurrentAttribIndex = 0;
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &TexCoords[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    mTexIndex = CurrentOffset;
    ++CurrentAttribIndex;

    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( glm::vec2 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Positions[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    mPosIndex = CurrentOffset;
    ++CurrentAttribIndex;

    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( GLfloat );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Headings[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    mHeadingIndex = CurrentOffset;
    ++CurrentAttribIndex;

    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( glm::vec2 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Sizes[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    mSizeIndex = CurrentOffset;
    ++CurrentAttribIndex;

    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( glm::vec4 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Colors[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    mColorIndex = CurrentOffset;
    mVAO.Unbind();
}

namespace {
void partitionByFilter( render::Counts_t& rv, RenderableSprites_t const& sprites, render::CountByTexId const& part, ActorRenderer::RenderFilter const& filter )
{
    rv.clear();
    size_t idx = part.Start;
    render::CountByTexId* actual = NULL;
    bool match = false;
    IRenderableComponent const* prevRC = nullptr;
    for( auto i = sprites.begin() + part.Start, e = sprites.begin() + part.Start + part.Count; i != e; ++i, ++idx )
    {
        auto const& val = *i;
        if( prevRC != val.RenderableComp )
        {
            match = filter( *val.RenderableComp );
            prevRC = val.RenderableComp;
        }
        if( !match )
        {
            actual = NULL;
        }
        else
        {
            if( NULL == actual )
            {
                rv.push_back( part );
                actual = &rv.back();
                actual->Start = idx;
                actual->Count = 0;
            }
            ++actual->Count;
        }
    }
}
}

void ActorRenderer::Draw( RenderFilter filter )
{
    mVAO.Bind();
    ShaderManager& ShaderMgr( ShaderManager::Get() );
    ShaderMgr.ActivateShader( "sprite2" );
    ShaderMgr.UploadData( "spriteTexture", GLuint( 1 ) );
    glActiveTexture( GL_TEXTURE0 + 1 );
    GLuint CurrentAttribIndex = 0;
    for( render::Counts_t::const_iterator i = mCounts.begin(), e = mCounts.end(); i != e; ++i )
    {
        render::CountByTexId const& BigPart = *i;
        glBindTexture( GL_TEXTURE_2D, BigPart.TexId );
        static render::Counts_t parts;
        partitionByFilter( parts, mRenderableSprites, BigPart, filter );
        for( auto const& Part : parts )
        {
            CurrentAttribIndex = 0;
            glVertexAttribPointer( CurrentAttribIndex, 4, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mTexIndex + sizeof( glm::vec4 )*Part.Start ) );
            glVertexAttribDivisor( CurrentAttribIndex, 1 );
            ++CurrentAttribIndex;
            glVertexAttribPointer( CurrentAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mPosIndex + sizeof( glm::vec2 )*Part.Start ) );
            glVertexAttribDivisor( CurrentAttribIndex, 1 );
            ++CurrentAttribIndex;
            glVertexAttribPointer( CurrentAttribIndex, 1, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mHeadingIndex + sizeof( GLfloat )*Part.Start ) );
            glVertexAttribDivisor( CurrentAttribIndex, 1 );
            ++CurrentAttribIndex;
            glVertexAttribPointer( CurrentAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mSizeIndex + sizeof( glm::vec2 )*Part.Start ) );
            glVertexAttribDivisor( CurrentAttribIndex, 1 );
            ++CurrentAttribIndex;
            glVertexAttribPointer( CurrentAttribIndex, 4, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mColorIndex + sizeof( glm::vec4 )*Part.Start ) );
            glVertexAttribDivisor( CurrentAttribIndex, 1 );
            glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, 4, Part.Count );
        }
    }
    glActiveTexture( GL_TEXTURE0 );
    mVAO.Unbind();
}

ActorRenderer::~ActorRenderer()
{

}

void ActorRenderer::OnMouseMoveEvent( const WorldMouseMoveEvent& Event )
{
    mX = Event.Pos.x;
    mY = Event.Pos.y;
}

bool ActorRenderer::RenderableSpriteCompare::operator()( RenderableSprite const& Rs1, RenderableSprite const& Rs2 )
{
    Opt<IRenderableComponent> Rs1RenderableC = Rs1.Obj->Get<IRenderableComponent>();
    Opt<IRenderableComponent> Rs2RenderableC = Rs2.Obj->Get<IRenderableComponent>();
    return Rs1RenderableC->GetLayer() < Rs2RenderableC->GetLayer() ||
           ( Rs1RenderableC->GetLayer() == Rs2RenderableC->GetLayer() &&
             ( Rs1RenderableC->GetZOrder() < Rs2RenderableC->GetZOrder() ||
               ( Rs1RenderableC->GetZOrder() == Rs2RenderableC->GetZOrder() &&
                 ( Rs1.ActId < Rs2.ActId ||
                   ( Rs1.ActId == Rs2.ActId &&
                     Rs1.Spr->TexId < Rs2.Spr->TexId ) ) ) ) );
}
