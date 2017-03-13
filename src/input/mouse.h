#ifndef INCLUDED_INPUT_MOUSE_H
#define INCLUDED_INPUT_MOUSE_H
#include "engine/system.h"
namespace engine {
class MouseSystem : public System
{
public:
    DEFINE_SYSTEM_BASE( MouseSystem )
    enum Button_t
    {
        Button_Left = GLFW_MOUSE_BUTTON_LEFT,
        Button_Right = GLFW_MOUSE_BUTTON_RIGHT,
        Button_Middle = GLFW_MOUSE_BUTTON_MIDDLE,
        Button_4 = GLFW_MOUSE_BUTTON_4,
        Button_5 = GLFW_MOUSE_BUTTON_5,
        Button_6 = GLFW_MOUSE_BUTTON_6,
        Button_7 = GLFW_MOUSE_BUTTON_7,
        Num_Buttons,
    };
    bool IsButtonPressed( Button_t Button )const;
    virtual void Init();
    virtual void Update( double DeltaTime );
    MouseSystem();
private:
    uint32_t mHeldButtons;
    glm::vec2 mRawPressCoord;
    glm::vec2 mRawMouseCoord;
    double mMousePressEventWait;
    const double mMousePressEventRepeatInterval;

    void MousePressed( int button, int action, int mods );

    static void OnMouseMove( GLFWwindow*, double x, double y );
    static void OnMouseButton( GLFWwindow*, int button, int action, int mods );
    static void OnMouseEnter( GLFWwindow*, int entered );
    static void OnMouseScroll( GLFWwindow*, double x_offs, double y_offs );
};
} // namespace engine
using engine::MouseSystem;
struct MouseMoveEvent : public Event
{
    const glm::vec2 Pos;
    MouseMoveEvent( const glm::vec2 P ): Pos( P ) {}
    virtual ~MouseMoveEvent() = 0;
};

struct ScreenMouseMoveEvent : public MouseMoveEvent
{
    ScreenMouseMoveEvent( const glm::vec2& P ): MouseMoveEvent( P ) {}
};

struct WorldMouseMoveEvent : public MouseMoveEvent
{
    WorldMouseMoveEvent( const glm::vec2& P ): MouseMoveEvent( P ) {}
};

struct UiMouseMoveEvent : public MouseMoveEvent
{
    UiMouseMoveEvent( const glm::vec2& P ): MouseMoveEvent( P ) {}
};

struct MousePressEvent : public Event
{
    const glm::vec2 Pos;
    const MouseSystem::Button_t Button;
    MousePressEvent( const glm::vec2& P, MouseSystem::Button_t B ): Pos( P ), Button( B ) {}
    virtual ~MousePressEvent() = 0;
};

struct ScreenMousePressEvent : public MousePressEvent
{
    ScreenMousePressEvent( const glm::vec2& P, MouseSystem::Button_t B ): MousePressEvent( P, B ) {}
};

struct WorldMousePressEvent : public MousePressEvent
{
    WorldMousePressEvent( const glm::vec2& P, MouseSystem::Button_t B ): MousePressEvent( P, B ) {}
};

struct UiMousePressEvent : public MousePressEvent
{
    UiMousePressEvent( const glm::vec2& P, MouseSystem::Button_t B ): MousePressEvent( P, B ) {}
};

struct MouseReleaseEvent : public Event
{
    const glm::vec2 Pos;
    const MouseSystem::Button_t Button;
    MouseReleaseEvent( const glm::vec2& P, MouseSystem::Button_t B ): Pos( P ), Button( B ) {}
    virtual ~MouseReleaseEvent() = 0;
};

struct ScreenMouseReleaseEvent : public MouseReleaseEvent
{
    ScreenMouseReleaseEvent( const glm::vec2& P, MouseSystem::Button_t B ): MouseReleaseEvent( P, B ) {}
};

struct WorldMouseReleaseEvent : public MouseReleaseEvent
{
    WorldMouseReleaseEvent( const glm::vec2& P, MouseSystem::Button_t B ): MouseReleaseEvent( P, B ) {}
};

struct UiMouseReleaseEvent : public MouseReleaseEvent
{
    UiMouseReleaseEvent( const glm::vec2& P, MouseSystem::Button_t B ): MouseReleaseEvent( P, B ) {}
};

struct MouseDragEvent : public Event
{
    const glm::vec2 From;
    const glm::vec2 To;
    const MouseSystem::Button_t Button;
    MouseDragEvent( const glm::vec2& F, const glm::vec2& T, MouseSystem::Button_t B ): From( F ), To( T ), Button( B ) {}
    virtual ~MouseDragEvent() = 0;
};

struct ScreenMouseDragEvent : public MouseDragEvent
{
    ScreenMouseDragEvent( const glm::vec2& F, const glm::vec2& T, MouseSystem::Button_t B ): MouseDragEvent( F, T, B ) {}
};

struct WorldMouseDragEvent : public MouseDragEvent
{
    WorldMouseDragEvent( const glm::vec2& F, const glm::vec2& T, MouseSystem::Button_t B ): MouseDragEvent( F, T, B ) {}
};

struct UiMouseDragEvent : public MouseDragEvent
{
    UiMouseDragEvent( const glm::vec2& F, const glm::vec2& T, MouseSystem::Button_t B ): MouseDragEvent( F, T, B ) {}
};

struct MouseEnterEvent : public Event
{
    const bool Enter;
    MouseEnterEvent( bool E ): Enter( E ) {}
};

struct MouseScrollEvent : public Event
{
    const glm::vec2 Offset;
    MouseScrollEvent( const glm::vec2& O ): Offset( O ) {}
};

#endif//INCLUDED_INPUT_MOUSE_H
