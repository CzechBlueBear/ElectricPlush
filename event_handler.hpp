#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <SDL2/SDL.h>

namespace plush {
    
    class EventHandler {
    public:
        
        /// Makes this event handler the current one, replacing the previous one.
        void makeCurrent();
        
        /// Returns a pointer to the current event handler.
        static EventHandler *current();
        
        virtual void onMouseMotion(SDL_MouseMotionEvent &event);
        virtual void onMouseButton(SDL_MouseButtonEvent &event);
        virtual void onKeyboard(SDL_KeyboardEvent &event);
        virtual void onIdle();
    };
    
} // namespace

#endif