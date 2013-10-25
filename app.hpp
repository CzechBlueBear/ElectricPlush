#ifndef APP_HPP
#define APP_HPP

#include "event_handler.hpp"

#include <SDL2/SDL.h>

namespace plush {
    
    struct RedrawTimerCallbackUserData {
        
        /**
         * Event type to generate when the redraw timer fires.
         * This is the value of App::m_redrawTimerEventID of the App.
         */
        Uint32 eventTypeToSend;
    };

    class App {
    protected:

        /// True if the app is properly initialized.
        bool m_ok;
        
        /// Window used for rendering.
        SDL_Window *m_window;

        /// OpenGL context.
        SDL_GLContext m_GLContext;

        /// Identifier for the redraw timer-generated message.
        Uint32 m_redrawTimerEventID;
        
        /// Screen redraw timer.
        SDL_TimerID m_redrawTimer;

        /// Data to pass to the timer.
        RedrawTimerCallbackUserData m_redrawTimerCallbackUserData;
        
        /// Redraw callback.
        void (*m_redrawFunc)();

        /// Keypress/keyrelease callback.
        void (*m_keyFunc)(const SDL_KeyboardEvent &event);
        
        /// Mouse motion callback.
        void (*m_mouseMotionFunc)(const SDL_MouseMotionEvent &event);
        
    public:
        
        App();
        ~App();
        
        /**
         * Returns true if the basic app is correctly initialized.
         * If not, the process should finish as soon as possible;
         * it is not guaranteed that GL context or window exist.
         */
        bool ok() const { return m_ok; }
        
        /**
         * The event loop.
         * Repeatedly processes events and calls the calbacks to handle them.
         * Returns when the application is requested to quit.
         */
        void eventLoop();
        
        /**
         * Called when the app window needs redrawing.
         */
        virtual void onRedraw();
        
        /**
         * Called when a key is pressed.
         */
        virtual void onKey(const SDL_KeyboardEvent &event);

        /**
         * Called when mouse motion is detected.
         */
        virtual void onMouseMotion(const SDL_MouseMotionEvent &event);
    };
}

#endif