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
        
        /// Redraw callback.
        static void (*redrawFunc)();

        /// Keypress/keyrelease callback.
        static void (*keyFunc)(const SDL_KeyboardEvent &event);
        
        /// Mouse motion callback.
        static void (*mouseMotionFunc)(const SDL_MouseMotionEvent &event);
        
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
         * Repeatedly processes events and calls the calbacks to handle them.
         * Returns when the application is requested to quit.
         */
        static void eventLoop();
        
        /**
         * Sets the redraw function which is responsible for redrawing the window
         * when neeed.
         * The redraw function is called from eventLoop().
         */
        static void setRedrawFunc(void (*fn)());
        
        /**
         * Sets the function to be called when a key press/release is detected.
         */
        static void setKeyFunc(void (*fn)(const SDL_KeyboardEvent&));
        
        /**
         * Sets the function to be called when a mouse motion is detected.
         */
        static void setMouseMotionFunc(void (*fn)(const SDL_MouseMotionEvent&));
    };
}

#endif