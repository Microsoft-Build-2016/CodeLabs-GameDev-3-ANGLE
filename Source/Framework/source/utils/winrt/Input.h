#pragma once

#include "pch.h"
#include "Game.h"
#include "Timer.h"

#include <memory>
#include <concurrent_queue.h>

namespace WinRT
{
    enum class PointerEventType
    {
        PointerPressed = 0,
        PointerMoved,
        PointerReleased
    };

    class PointerEvent
    {
    public:
        PointerEvent(PointerEventType type, GLfloat x, GLfloat y, int id = 0)
            : _type(type)
            , _x(x)
            , _y(y)
            , _id(id)
        {}

        PointerEventType _type;
        GLfloat _x;
        GLfloat _y;
        int _id;
    };

    enum class KeyEventType
    {
        KeyDown = 0,
        KeyUp
    };

    class KeyEvent
    {
    public:
        KeyEvent(KeyEventType type, Windows::UI::Core::KeyEventArgs^ args)
            : _type(type)
            , m_key(args)
        {}

        KeyEventType _type;
        Platform::Agile<Windows::UI::Core::KeyEventArgs> m_key;
    };

    class Input
    {
    public:
        Input();
        ~Input();
        void QueuePointerEvent(PointerEventType type, GLfloat x, GLfloat y, unsigned int id);
        void QueueKeyEvent(KeyEventType type, Windows::UI::Core::KeyEventArgs^ args);
        void ProcessEvents();

    private:
        void ProcessPointerEvents();
        virtual void OnPointerPressed(std::shared_ptr<PointerEvent> e) = 0;
        virtual void OnPointerMoved(std::shared_ptr<PointerEvent> e) = 0;
        virtual void OnPointerReleased(std::shared_ptr<PointerEvent> e) = 0;

        void ProcessKeyEvents();
        virtual void OnKeyDown(std::shared_ptr<KeyEvent> e) = 0;
        virtual void OnKeyUp(std::shared_ptr<KeyEvent> e) = 0;

        Concurrency::concurrent_queue<std::shared_ptr<PointerEvent>> mPointerEvents;
        Concurrency::concurrent_queue<std::shared_ptr<KeyEvent>> mKeyEvents;
    };
}