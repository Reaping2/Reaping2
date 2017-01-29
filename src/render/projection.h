#ifndef INCLUDED_RENDER_PROJECTION_H
#define INCLUDED_RENDER_PROJECTION_H
#include "platform/i_platform.h"
#include "main/window.h"

struct Viewport
{
    GLint X;
    GLint Y;
    GLsizei Width;
    GLsizei Height;
    Viewport(): X( 0 ), Y( 0 ), Width( 0 ), Height( 0 ) {}
    glm::vec2 Size() const { return glm::vec2( Width, Height ); }
};

class Projection
{
public:
    enum ViewMode
    {
        VM_Fixed,
        VM_DynamicRatio,
    };
    const glm::mat4& GetMatrix()const;
    const glm::vec4& GetVisibleRegion()const;
    const Viewport& GetViewport()const;
    const glm::mat4& GetInverseMatrix()const;
    float GetRatio()const;
    glm::vec3 Unproject( glm::vec3& Vec ) const;
    Projection( float bottom, float top, ViewMode vm = VM_DynamicRatio, float nearVal = 1.0f, float farVal = -1.0f );
    void Setup( float bottom, float top, ViewMode vm = VM_DynamicRatio, float nearVal = 1.0f, float farVal = -1.0f );
protected:
    void OnWindowResizeEvent( const WindowResizeEvent& Event );
    void Resize( int Width, int Height );
    void SetupMatrices();
    ViewMode mViewMode;
    glm::mat4 mMatrix;
    glm::mat4 mInverseMatrix;
    Viewport mViewport;
    glm::vec4 mViewportTransf;
    static const glm::mat4 mIdentity;

    AutoReg mWindowResizeId;

    float mRatio;

    glm::vec4 mVisibleRegion;
    float mNearVal;
    float mFarVal;
};

#endif//INCLUDED_RENDER_PROJECTION_H

