//
// This file is used by the template to render a basic scene using GL.
//

#include "pch.h"
#include "Input.h"

using namespace Platform;
using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace WinRT;

Input::Input()
{
}

Input::~Input()
{

}

void Input::ProcessEvents()
{
    ProcessKeyEvents();
    ProcessPointerEvents();
}


void Input::QueuePointerEvent(PointerEventType type, GLfloat x, GLfloat y, unsigned int id)
{
    std::shared_ptr<PointerEvent> e(new PointerEvent(type, x, y, id));
    mPointerEvents.push(e);
}

void Input::ProcessPointerEvents()
{
    std::shared_ptr<PointerEvent> e;
    while (mPointerEvents.try_pop(e))
    {
        switch (e->_type)
        {
        case PointerEventType::PointerPressed:
            OnPointerPressed(e);
            break;

        case PointerEventType::PointerMoved:
            OnPointerMoved(e);
            break;

        case PointerEventType::PointerReleased:
            OnPointerReleased(e);
            break;

        default:
            break;
        }
    }
}

void Input::QueueKeyEvent(KeyEventType type, KeyEventArgs^ args)
{
    std::shared_ptr<KeyEvent> e(new KeyEvent(type, args));
    mKeyEvents.push(e);
}

void Input::ProcessKeyEvents()
{
    std::shared_ptr<KeyEvent> e;
    while (mKeyEvents.try_pop(e))
    {
        switch (e->_type)
        {
        case KeyEventType::KeyDown:
            OnKeyDown(e);
            break;

        case KeyEventType::KeyUp:
            OnKeyUp(e);
            break;

        default:
            break;
        }
    }
}

