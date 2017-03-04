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
#include "engine/activity_system.h"
#include "core/program_state.h"
#include "core/scene.h"
#include "renderable_repo.h"
#include "i_visual_box_multiplier_component.h"
#include "sprite_phase_cache.h"
#include "platform/game_clock.h"
#include "main/window.h"
#include <boost/ref.hpp>
#include "action_renderer_loader_repo.h"
#include "core/perf_timer.h"

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
    , mSecondaryTexIndex(0)
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
bool isVisible( Actor const& actor, std::vector<Camera const*> const& cameras )
{
    Opt<IPositionComponent> const positionC = actor.Get<IPositionComponent>();
    if( !positionC.IsValid() )
    {
        return false;
    }
    Opt<IRenderableComponent> const renderableC = actor.Get<IRenderableComponent>();
    if( !renderableC.IsValid() )
    {
        return false;
    }
    static RenderableRepo& renderables( RenderableRepo::Get() );
    float scale = renderables.GetMaxScale( actor.GetId() );
    Opt<ICollisionComponent> const collisionC = actor.Get<ICollisionComponent>();
    glm::vec2 const& visMulti = visMultiplier( actor );
    float vmult = std::max<float>( visMulti.x, visMulti.y );
    // 2.0 multiplier: safety
    float size = ( collisionC.IsValid() ? collisionC->GetRadius() : 50 ) * scale * 2.0 * vmult;
    for( auto camera : cameras )
    {
        glm::vec4 const& region = camera->VisibleRegion();
        if( region.x < positionC->GetX() + size && region.z > positionC->GetX() - size
            && region.y < positionC->GetY() + size && region.w > positionC->GetY() - size )
        {
            return true;
        }
    }
    return false;
}
bool getNextTextId( RenderableSprites_t::const_iterator& i, RenderableSprites_t::const_iterator e,
                    glm::vec2*& Positions, GLfloat*& Headings,
                    glm::vec4*& TexCoords, glm::vec4*& SecondaryTexCoords,
                    glm::vec4*& MaskTexCoords, glm::vec4*& NormalTexCoords,
                    glm::vec2*& Sizes, glm::vec4*& Colors,
                    std::map<int32_t, std::vector<glm::vec4> >& postprocColors,
                    GLfloat*& ShadowLenScales,
                    size_t& cnt,
                    render::RenderBatch& batch )
{
    static int32_t prevNormalId = -1;
    static int32_t prevsecid = -1;
    if( i == e )
    {
        prevNormalId = -1;
        prevsecid = -1;
        return false;
    }
    batch.TexId = i->Spr->TexId;
    // mask id is by default the tex id ( if no additional alpha masking is necessary )
    batch.MaskId = batch.TexId;
    batch.NormalId = prevNormalId;
    // we don't swap back from a valid sec. tex id to -1
    // so there are less RenderBatch changes in render::count
    batch.SecondaryTexId = prevsecid;
    batch.ShaderId = i->RenderableComp->GetShaderId();
    (*Positions++) = glm::vec2( i->PositionC->GetX(), i->PositionC->GetY() ) + i->RelativePosition;
    (*Headings++) = ( GLfloat )i->PositionC->GetOrientation() + i->RelativeOrientation;
    (*ShadowLenScales++) = (GLfloat)i->RenderableComp->GetShadowLenScale();

    float const radius = (( i->CollisionC != nullptr ? i->CollisionC->GetRadius() : 50 )
                            + i->RelativeRadius) *i->Anim->GetScale();
    auto const size = radius * visMultiplier( *(i->Obj) );
    (*Sizes) = size;
    ++Sizes;

    glm::vec4 secondaryTexCoord(-1,-1,-1,-1);
    SpritePhase const* Spr = nullptr;
    if( !i->AdditionalSprs.empty() )
    {
        Spr = i->AdditionalSprs.front();
        secondaryTexCoord = glm::vec4( Spr->Left, Spr->Right, Spr->Bottom, Spr->Top );
        prevsecid = batch.SecondaryTexId = Spr->TexId;
    }
    (*SecondaryTexCoords++) = secondaryTexCoord;
    auto coords = glm::vec4( i->Spr->Left, i->Spr->Right, i->Spr->Bottom, i->Spr->Top );
    (*TexCoords++) = coords;
    if( i->MaskSpr != nullptr )
    {
        batch.MaskId = i->MaskSpr->TexId;
        coords = glm::vec4( i->MaskSpr->Left, i->MaskSpr->Right, i->MaskSpr->Bottom, i->MaskSpr->Top );
    }
    (*MaskTexCoords++) = coords;
    coords = glm::vec4( -1, -1, -1, -1 );
    if( i->NormalSpr != nullptr )
    {
        prevNormalId = batch.NormalId = i->NormalSpr->TexId;
        coords = glm::vec4( i->NormalSpr->Left, i->NormalSpr->Right, i->NormalSpr->Bottom, i->NormalSpr->Top );
    }
    (*NormalTexCoords++) = coords;
    (*Colors++) = i->Color;
    {   // move to cache
        static render::SpritePhaseCache& cache( render::SpritePhaseCache::Get() );
        cache.Request( *i->Spr, std::max( size.x, size.y ) );
        if( Spr != nullptr )
        {
            cache.Request( *Spr, std::max( size.x, size.y ) );
        }
        if( i->MaskSpr != nullptr )
        {
            cache.Request( *i->MaskSpr, std::max( size.x, size.y ) );
        }
        if( i->NormalSpr != nullptr )
        {
            cache.Request( *i->NormalSpr, std::max( size.x, size.y ) );
        }
    }
    for( auto id : i->RenderableComp->GetPostProcessIds() )
    {
        postprocColors[ id ][cnt] = glm::vec4(1,1,1,1);
    }
    ++i;
    ++cnt;
    return true;
}
}

void ActorRenderer::Prepare( Scene const& , std::vector<Camera const*> const& cameras, double DeltaTime )
{
    static auto activityS = engine::Engine::Get().GetSystem<engine::ActivitySystem>();
    auto const& Lst = activityS->GetActiveActors();
    if( Lst.empty() )
    {
        mRenderableSprites.clear(); // renderable sprites still can contain obsolete sprites, so render nothing instead of invalid object
        mCounts.clear();
        mPostprocessColors.clear();
        return;
    }
    L2( "Lst size: %d", Lst.size() );
    RenderableSprites_t RenderableSprites;
    RenderableSprites.reserve( mRenderableSprites.size() );
    std::set<int32_t> postprocids;

    for( auto i = Lst.begin(), e = Lst.end(); i != e; ++i )
    {
        const Actor& Object = **i;
        if( !isVisible( Object, cameras ) )
        {
            continue;
        }
        auto recogptr = mRecognizerRepo.GetRecognizers( Object.GetId() );
        if( nullptr == recogptr || recogptr->empty() )
        {
            continue;
        }
        Opt<IRenderableComponent> renderableC( Object.Get<IRenderableComponent>() );
        auto const& pp = renderableC->GetPostProcessIds();
        postprocids.insert( pp.begin(), pp.end() );
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
                    static auto& mActionRendererLoaderRepo( render::ActionRendererLoaderRepo::Get() );
                    std::auto_ptr<ActionRenderer> actionRenderer( mActionRendererFactory( actionRendererId ) );
                    actionRenderer->SetOrder( recognizer.GetOrder() );
                    actionRenderer->Init( Object );
                    auto const& loader = mActionRendererLoaderRepo(Object.GetId(), actionRendererId );
                    actionRenderer = loader.FillProperties( actionRenderer );
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
        mCounts.clear();
        mPostprocessColors.clear();
        return;
    }

    Positions_t Positions( CurSize );
    Floats_t Headings( CurSize );
    Floats_t ShadowLenScales( CurSize );
    Positions_t Sizes( CurSize );
    TexCoords_t TexCoords( CurSize );
    TexCoords_t SecondaryTexCoords( CurSize );
    TexCoords_t MaskTexCoords( CurSize );
    TexCoords_t NormalTexCoords( CurSize );
    Colors_t Colors( CurSize );
    postprocids.insert( -1 );
    mPostprocessColors.clear();
    for( auto id : postprocids )
    {
        mPostprocessColors[ id ] = Colors_t( CurSize, glm::vec4(0,0,0,1) );
    }

    glm::vec2* posptr = &Positions[0];
    GLfloat* hptr = &Headings[0];
    GLfloat* shadowlenscaleptr = &ShadowLenScales[0];
    glm::vec2* sptr = &Sizes[0];
    glm::vec4* tptr = &TexCoords[0];
    glm::vec4* stptr = &SecondaryTexCoords[0];
    glm::vec4* mtptr = &MaskTexCoords[0];
    glm::vec4* ntptr = &NormalTexCoords[0];
    glm::vec4* cptr = &Colors[0];
    size_t cnt = 0;
    RenderableSprites_t::const_iterator i = mRenderableSprites.begin();
    mCounts = render::count(
        std::bind( &getNextTextId, std::ref( i ), mRenderableSprites.end(),
        std::ref( posptr ), std::ref( hptr ), std::ref( tptr ),
        std::ref( stptr ), std::ref(mtptr), std::ref(ntptr),
        std::ref( sptr ), std::ref( cptr ), std::ref( mPostprocessColors ),
        std::ref( shadowlenscaleptr ),
        std::ref( cnt ),
        std::placeholders::_1 )
    );
    mVAO.Bind();

    if( CurSize > mPrevSize )
    {
        mPrevSize = CurSize;
        size_t TotalSize = CurSize * ( 2 * sizeof( glm::vec4 ) + 2 * sizeof( glm::vec2 ) + 2 * sizeof( GLfloat ) + 4 * sizeof( glm::vec4 ) );
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
    CurrentSize = CurSize * sizeof( glm::vec4 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &SecondaryTexCoords[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    mSecondaryTexIndex = CurrentOffset;
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
    ++CurrentAttribIndex;

    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( glm::vec4 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &MaskTexCoords[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    mMaskTexCoordIndex = CurrentOffset;
    ++CurrentAttribIndex;

    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( glm::vec4 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &NormalTexCoords[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    mNormalTexCoordIndex = CurrentOffset;
    ++CurrentAttribIndex;

    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( GLfloat );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &ShadowLenScales[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    mShadowLenScaleIndex = CurrentOffset;
    ++CurrentAttribIndex;

    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( glm::vec4 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &mPostprocessColors[-1][0] );
    // no need to actually upload this, random mem is just fine
    // although it might be faster to just upload everything at prepare time
    glEnableVertexAttribArray( CurrentAttribIndex );
    mPostprocessColorIndex = CurrentOffset;
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

void ActorRenderer::Draw( RenderFilter filter, SetupFunction setup, size_t extraInstances )
{
    if( mRenderableSprites.empty() )
    {
        return;
    }
    mVAO.Bind();
    ShaderManager& ShaderMgr( ShaderManager::Get() );
    static int32_t def = AutoId( "sprite2" );
    ShaderMgr.SetDefaultShader( def );
    for( render::Counts_t::const_iterator i = mCounts.begin(), e = mCounts.end(); i != e; ++i )
    {
        render::CountByTexId const& BigPart = *i;
        ShaderMgr.ActivateShader( BigPart.Batch.ShaderId );
        glActiveTexture( GL_TEXTURE0 + 1 );
        glBindTexture( GL_TEXTURE_2D, BigPart.Batch.TexId );
        ShaderMgr.UploadData( "spriteTexture", GLuint( 1 ) );
        if( BigPart.Batch.NormalId != -1 )
        {
            glActiveTexture( GL_TEXTURE0 + 2 );
            glBindTexture( GL_TEXTURE_2D, BigPart.Batch.NormalId );
            ShaderMgr.UploadData( "normalTexture", GLuint( 2 ) );
        }
        if( BigPart.Batch.SecondaryTexId != -1 )
        {
            glActiveTexture( GL_TEXTURE0 + 3 );
            glBindTexture( GL_TEXTURE_2D, BigPart.Batch.SecondaryTexId );
            ShaderMgr.UploadData( "secondarySpriteTexture", GLuint( 3 ) );
        }
        ShaderMgr.UploadData( "time", GLfloat( platform::Clock::Now() ) );
        int w, h;
        static auto Window = engine::Engine::Get().GetSystem<engine::WindowSystem>();
        Window->GetWindowSize( w, h );
        ShaderMgr.UploadData( "resolution", glm::vec2( w, h ) );
        if( setup )
        {
            setup( ShaderMgr );
        }
        render::Counts_t parts;
        partitionByFilter( parts, mRenderableSprites, BigPart, filter );
        for( auto const& Part : parts )
        {
            DrawOnePart( Part, 4 + extraInstances * 4 );
        }
    }
    glActiveTexture( GL_TEXTURE0 );
    mVAO.Unbind();
}

void ActorRenderer::Draw( int32_t postprocid )
{
    if( mRenderableSprites.empty() || mCounts.empty() )
    {
        return;
    }
    auto ppcit = mPostprocessColors.find( postprocid );
    if( ppcit == mPostprocessColors.end() )
    {
        return;
    }
    auto const& ppc = ppcit->second;
    if( ppc.empty() )
    {
        return;
    }
    mVAO.Bind();
    ShaderManager& ShaderMgr( ShaderManager::Get() );
    static int32_t def = AutoId( "postprocessor_select" );
    ShaderMgr.ActivateShader( def );
    glBufferSubData( GL_ARRAY_BUFFER, mPostprocessColorIndex, sizeof(glm::vec4) * ppc.size(), &ppc[0] );
    for( auto const& BigPart : mCounts )
    {
        glActiveTexture( GL_TEXTURE0 + 1 );
        glBindTexture( GL_TEXTURE_2D, BigPart.Batch.MaskId );
        ShaderMgr.UploadData( "spriteTexture", GLuint( 1 ) );
        DrawOnePart( BigPart, 4 );
    }
    glActiveTexture( GL_TEXTURE0 );
    mVAO.Unbind();
}

void ActorRenderer::DrawOnePart( render::CountByTexId const& Part, size_t instances ) const
{
    GLuint CurrentAttribIndex = 0;
    CurrentAttribIndex = 0;
    glVertexAttribPointer( CurrentAttribIndex, 4, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mTexIndex + sizeof( glm::vec4 )*Part.Start ) );
    glVertexAttribDivisor( CurrentAttribIndex, 1 );
    ++CurrentAttribIndex;
    glVertexAttribPointer( CurrentAttribIndex, 4, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mSecondaryTexIndex + sizeof( glm::vec4 )*Part.Start ) );
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
    ++CurrentAttribIndex;
    glVertexAttribPointer( CurrentAttribIndex, 4, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mMaskTexCoordIndex + sizeof( glm::vec4 )*Part.Start ) );
    glVertexAttribDivisor( CurrentAttribIndex, 1 );
    ++CurrentAttribIndex;
    glVertexAttribPointer( CurrentAttribIndex, 4, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mNormalTexCoordIndex + sizeof( glm::vec4 )*Part.Start ) );
    glVertexAttribDivisor( CurrentAttribIndex, 1 );
    ++CurrentAttribIndex;
    glVertexAttribPointer( CurrentAttribIndex, 1, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mShadowLenScaleIndex + sizeof( GLfloat )*Part.Start ) );
    glVertexAttribDivisor( CurrentAttribIndex, 1 );
    ++CurrentAttribIndex;
    glVertexAttribPointer( CurrentAttribIndex, 4, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mPostprocessColorIndex + sizeof( glm::vec4 )*Part.Start ) );
    glVertexAttribDivisor( CurrentAttribIndex, 1 );
    glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, instances, Part.Count );
}

ActorRenderer::~ActorRenderer()
{

}

void ActorRenderer::OnMouseMoveEvent( const WorldMouseMoveEvent& Event )
{
    mX = Event.Pos.x;
    mY = Event.Pos.y;
}

