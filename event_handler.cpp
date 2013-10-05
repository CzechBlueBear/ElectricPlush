#include "event_handler.hpp"

using namespace plush;

static EventHandler* currentEventHandler = nullptr;

void EventHandler::makeCurrent()
{
    currentEventHandler = this;
}

EventHandler* EventHandler::current()
{
    return currentEventHandler;
}

void EventHandler::onMouseMotion(SDL_MouseMotionEvent &event)
{
}

void EventHandler::onMouseButton(SDL_MouseButtonEvent &event)
{
}

void EventHandler::onKeyboard(SDL_KeyboardEvent &event)
{
}

void EventHandler::onIdle()
{
}
