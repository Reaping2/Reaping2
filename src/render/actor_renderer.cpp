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

void ActorRenderer::Init()
{
    mVAO.Init();
    mOnActorEvent = EventServer<ActorEvent>::Get().Subscribe( boost::bind( &ActorRenderer::OnActorEvent, this, _1 ) );
    mMouseMoveId = EventServer<WorldMouseMoveEvent>::Get().Subscribe( boost::bind( &ActorRenderer::OnMouseMoveEvent, this, _1 ) );
}

void ActorRenderer::OnActorEvent(ActorEvent const& Evt)
{
    if(Evt.mState==ActorEvent::Added)
    {
        mActionRenderersMap[Evt.mActor->GetGUID()];
    }
    else
    {
        mActionRenderersMap.erase(Evt.mActor->GetGUID());
    }
}

ActorRenderer::ActorRenderer()
    : mRecognizerRepo(RecognizerRepo::Get())
    , mActionRendererFactory(ActionRendererFactory::Get())
{
    Init();
}

void ActorRenderer::Draw( Scene const& Object, double DeltaTime )
{
    ActorList_t const& Lst = Object.GetActors();
    if( Lst.empty() )
    {
        return;
    }
    static size_t PrevVecSize = 0;
    typedef ActionRenderer::RenderableSprites_t RenderableSprites_t;
    RenderableSprites_t RenderableSprites;
    RenderableSprites.reserve( PrevVecSize );
    //the template version works well with '=' i just dont know is it really needed, maybe this one is more self explaining
    ActorListFilter<Scene::RenderableActors> wrp(Lst);//=Object.GetActors<Scene::RenderableComponents>();
    for(ActorListFilter<Scene::RenderableActors>::const_iterator i=wrp.begin(),e=wrp.end();i!=e;++i)
    {
        const Actor& Object = **i;
        if (mRecognizerRepo.HasRecognizers(Object.GetId()))
        {
            RecognizerRepo::Recognizers_t& recognizers=mRecognizerRepo.GetRecognizers(Object.GetId());
            RecognizerRepo::ExcludedRecognizers_t excluded;

            ActionRenderersMap_t::iterator actionRenderersIt = mActionRenderersMap.find(Object.GetGUID());
            BOOST_ASSERT(actionRenderersIt!=mActionRenderersMap.end());
            ActionRenderers_t& actionRenderers=actionRenderersIt->second;

            for (RecognizerRepo::Recognizers_t::iterator recogIt=recognizers.begin(),recogE=recognizers.end(); recogIt!=recogE;++recogIt)
            {
                render::Recognizer& recognizer=*recogIt;
                if (excluded.find(recognizer.GetId())==excluded.end()
                    &&recognizer.Recognize(Object))
                {
                    int32_t actionRendererId = recognizer.GetActionRenderer();
                    ActionRenderers_t::iterator foundActionRendererIt = 
                        std::find_if(actionRenderers.begin(),actionRenderers.end(), FindActionRenderer(actionRendererId));
                    if (foundActionRendererIt==actionRenderers.end())
                    {
                        std::auto_ptr<ActionRenderer> actionRenderer(mActionRendererFactory(actionRendererId));
                        actionRenderer->SetOrder(recognizer.GetOrder());
                        actionRenderer->Init(Object);
                        actionRenderers.insert(actionRenderer);
                    }
                    if (mRecognizerRepo.HasExcludedRecognizers(recognizer.GetId()))
                    {
                        RecognizerRepo::ExcludedRecognizers_t& excludedRecognizers=mRecognizerRepo.GetExcludedRecognizers(recognizer.GetId());
                        excluded.insert(excludedRecognizers.begin(),excludedRecognizers.end());
                    }
                }
            }
            for (RecognizerRepo::ExcludedRecognizers_t::iterator excludedIt=excluded.begin(), excludedE=excluded.end();excludedIt!=excludedE;++excludedIt)
            {
                ActionRenderers_t::iterator arIt=std::find_if(actionRenderers.begin(),actionRenderers.end(), FindActionRenderer(*excludedIt));
                if(arIt!=actionRenderers.end())
                {
                    actionRenderers.erase(arIt);

                }
            }
            for (ActionRenderers_t::iterator actionRendererIt=actionRenderers.begin(), actionRendererE=actionRenderers.end();actionRendererIt!=actionRendererE;++actionRendererIt)
            {
                ActionRenderer& actionRenderer=*actionRendererIt;
                actionRenderer.FillRenderableSprites(Object,RenderableSprites);
                actionRenderer.Update(DeltaTime);
            }
        }
    }

    // TODO: sort Z order, alive state es texture id alapjan.
    // Meg persze (last cmp) pointerek szerint, hogy determinisztikus legyen.
    //std::sort( RenderableSprites.begin(), RenderableSprites.end(), RenderableSpriteCompare() );

    size_t CurSize = RenderableSprites.size();
    typedef std::vector<glm::vec2> Positions_t;
    Positions_t Positions;
    Positions.reserve( CurSize );
    typedef std::vector<GLfloat> Floats_t;
    Floats_t Headings;
    Headings.reserve( CurSize );
    Floats_t Sizes;
    Sizes.reserve( CurSize );
    typedef std::vector<glm::vec4> TexCoords_t;
    TexCoords_t TexCoords;
    TexCoords.reserve( CurSize );

    typedef std::vector<glm::vec4> Colors_t;
    Colors_t Colors;
    Colors.reserve( CurSize );

    Counts_t Counts;
    GLuint TexId = -1;
    size_t Count = 0;
    size_t Start = 0;
    size_t MaxCount = 0;
    for( RenderableSprites_t::const_iterator i = RenderableSprites.begin(), e = RenderableSprites.end(); i != e; ++i )
    {
        if( TexId != i->Spr->TexId )
        {
            if( Count )
            {
                Counts.push_back( CountByTexId( TexId, Start, Count ) );
            }
            Start += Count;
            Count = 0;
            TexId = i->Spr->TexId;
        }
        ++Count;
        if( Count > MaxCount )
        {
            MaxCount = Count;
        }
        Opt<IPositionComponent> const positionC = i->Obj->Get<IPositionComponent>();
        Positions.push_back( glm::vec2( positionC->GetX(), positionC->GetY() ) );
        Headings.push_back( ( GLfloat )positionC->GetOrientation() );

        Opt<ICollisionComponent> const collisionC = i->Obj->Get<ICollisionComponent>();
        //TODO: this one should not depend on collision radius
        Sizes.push_back( ( GLfloat )( (collisionC.IsValid()?collisionC->GetRadius():50)*i->Anim->GetScale() ) ); 
        TexCoords.push_back( glm::vec4( i->Spr->Left, i->Spr->Right, i->Spr->Bottom, i->Spr->Top ) );
        Colors.push_back( i->Color );

    }
    if( Count )
    {
        Counts.push_back( CountByTexId( TexId, Start, Count ) );
    }
    if( CurSize == 0 )
    {
        return;
    }

    mVAO.Bind();
    if( CurSize != PrevVecSize )
    {
        PrevVecSize = CurSize;
        size_t TotalSize = CurSize * ( sizeof( glm::vec4 ) + sizeof( glm::vec2 ) + 2 * sizeof( GLfloat ) + sizeof( glm::vec4 ) );
        glBufferData( GL_ARRAY_BUFFER, TotalSize, NULL, GL_DYNAMIC_DRAW );
    }

    size_t CurrentOffset = 0;
    size_t CurrentSize = CurSize * sizeof( glm::vec4 );
    GLuint CurrentAttribIndex = 0;
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &TexCoords[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    size_t const TexIndex = CurrentOffset;
    ++CurrentAttribIndex;

    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( glm::vec2 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Positions[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    size_t const PosIndex = CurrentOffset;
    ++CurrentAttribIndex;

    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( GLfloat );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Headings[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    size_t const HeadingIndex = CurrentOffset;
    ++CurrentAttribIndex;

    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( GLfloat );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Sizes[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    size_t const SizeIndex = CurrentOffset;
    ++CurrentAttribIndex;

    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( glm::vec4 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Colors[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    size_t const ColorIndex = CurrentOffset;

    ShaderManager& ShaderMgr( ShaderManager::Get() );
    ShaderMgr.ActivateShader( "sprite2" );
    ShaderMgr.UploadData( "spriteTexture", GLuint( 1 ) );
    glActiveTexture( GL_TEXTURE0 + 1 );
    for( Counts_t::const_iterator i = Counts.begin(), e = Counts.end(); i != e; ++i )
    {
        CountByTexId const& Part = *i;
        CurrentAttribIndex = 0;
        glVertexAttribPointer( CurrentAttribIndex, 4, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( TexIndex + sizeof( glm::vec4 )*Part.Start ) );
        glVertexAttribDivisor( CurrentAttribIndex, 1 );
        ++CurrentAttribIndex;
        glVertexAttribPointer( CurrentAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( PosIndex + sizeof( glm::vec2 )*Part.Start ) );
        glVertexAttribDivisor( CurrentAttribIndex, 1 );
        ++CurrentAttribIndex;
        glVertexAttribPointer( CurrentAttribIndex, 1, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( HeadingIndex + sizeof( GLfloat )*Part.Start ) );
        glVertexAttribDivisor( CurrentAttribIndex, 1 );
        ++CurrentAttribIndex;
        glVertexAttribPointer( CurrentAttribIndex, 1, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( SizeIndex + sizeof( GLfloat )*Part.Start ) );
        glVertexAttribDivisor( CurrentAttribIndex, 1 );
        ++CurrentAttribIndex;
        glVertexAttribPointer( CurrentAttribIndex, 4, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( ColorIndex + sizeof( glm::vec4 )*Part.Start ) );
        glVertexAttribDivisor( CurrentAttribIndex, 1 );
        glBindTexture( GL_TEXTURE_2D, Part.TexId );
        glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, 4, Part.Count );
    }
    glActiveTexture( GL_TEXTURE0 );
    mVAO.Unbind();
}

ActorRenderer::~ActorRenderer()
{

}

void ActorRenderer::OnMouseMoveEvent(const WorldMouseMoveEvent& Event)
{
    mX = Event.Pos.x;
    mY = Event.Pos.y;
}

bool ActorRenderer::RenderableSpriteCompare::operator()( RenderableSprite const& Rs1, RenderableSprite const& Rs2 )
{
    Opt<IRenderableComponent> Rs1RenderableC = Rs1.Obj->Get<IRenderableComponent>();
    Opt<IRenderableComponent> Rs2RenderableC = Rs2.Obj->Get<IRenderableComponent>();
    return Rs1RenderableC->GetLayer()< Rs2RenderableC->GetLayer()||
        ( Rs1RenderableC->GetLayer()== Rs2RenderableC->GetLayer()&&
        ( Rs1RenderableC->GetZOrder()< Rs2RenderableC->GetZOrder()||
        ( Rs1RenderableC->GetZOrder()< Rs2RenderableC->GetZOrder() &&
        ( Rs1.ActId < Rs2.ActId ||
        ( Rs1.ActId == Rs2.ActId &&
        Rs1.Spr->TexId < Rs2.Spr->TexId ) ) ) ) );
}
