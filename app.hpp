#ifndef APP_HPP
#define APP_HPP

#include "event_handler.hpp"

#include <SDL2/SDL.h>

namespace plush {
    class App {
    protected:

        /**
         * Window used for graphics.
         */
        static SDL_Window *theWindow;

        /**
         * OpenGL context.
         */
        static SDL_GLContext theGLContext;
        
        /**
         * Pointer to the redraw function.
         */
        static void (*redrawFunc)();

        /// Keypress/keyrelease callback.
        static void (*keyFunc)(const SDL_KeyboardEvent &event);
        
    public:
        
        /**
         * Initializes the application.
         * Returns true on success, false on failure.
         * Failure details are reported on standard error output.
         */
        static bool init();

        /**
         * Finalizes the application. Should be called before terminating.
         */
        static void finish();
        
        /**
         * The event loop.
         * Repeatedly processes events and calls the current event handler to handle them.
         * Returns when the application is requested to quit.
         */
        static void eventLoop();
        
        /**
         * Sets the redraw function which is responsible for redrawing the window
         * when neeed.
         * The redraw function is called from eventLoop().
         */
        static void setRedrawFunc(void (*fn)());
        
        static void setKeyFunc(void (*fn)(const SDL_KeyboardEvent&));
    };
}

#endif