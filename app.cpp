#include "app.hpp"
#include "gl.hpp"

#include <iostream>
#include <stdexcept>

using namespace plush;

SDL_Window *App::theWindow = nullptr;
SDL_GLContext App::theGLContext = nullptr;

/////////////////////////////////////////////////////////////////////////////
// Redraw-related stuff
/////////////////////////////////////////////////////////////////////////////

/**
 * Registered event type for the timer event.
 */
Uint32 redrawTimerEventType;

/**
 * ID of the timer used for periodic redraws.
 */
SDL_TimerID redrawTimer;

/**
 * How often the timer requests a redraw (25 ms -> 40 frames per second).
 */
const Uint32 REDRAW_TIMER_INTERVAL = 25;

/**
 * Pointer to function to be called to redraw the screen.
 */
void (*App::redrawFunc)() = nullptr;

/**
 * Callback that is executed in the timer thread when the redraw timer fires.
 * Only adds an event to the event queue and returns; the redrawing is handled
 * in the main thread (App::eventLoop()).
 */
static Uint32 redrawTimerCallback(Uint32 interval, void *userData)
{
    SDL_Event event;
    event.type = redrawTimerEventType;
    event.user.code = 0;
    event.user.data1 = nullptr;
    event.user.data2 = nullptr;
    
    SDL_PushEvent(&event);

    return interval;
}

/////////////////////////////////////////////////////////////////////////////
// App
/////////////////////////////////////////////////////////////////////////////

bool App::init()
{
    GLenum err;
    
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << "\n";
        return false;
    }

    theWindow = SDL_CreateWindow("Electric Plush",
                                 
                                 // let the window manager place this window
                                 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                 
                                 1024, 768,
                                 SDL_WINDOW_OPENGL);
    if (!theWindow) {
        std::cerr << "Error opening window: " << SDL_GetError() << "\n";
        goto fail;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    theGLContext = SDL_GL_CreateContext(theWindow);
    if (!theGLContext) {
        std::cerr << "Error creating SDL GL context: " << SDL_GetError() << "\n";
        goto fail;
    }

    SDL_GL_MakeCurrent(theWindow, theGLContext);
    
    err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << "\n";
        goto fail;
    }
    
    redrawTimerEventType = SDL_RegisterEvents(1);
    if (redrawTimerEventType == (Uint32)(-1)) {
        std::cerr << "Error: SDL_RegisterEvents(): " << SDL_GetError() << "\n";
        goto fail;
    }
    
    redrawTimer = SDL_AddTimer(REDRAW_TIMER_INTERVAL, redrawTimerCallback, nullptr);
    if (!redrawTimer) {
        std::cerr << "Error: SDL_AddTimer(): " << SDL_GetError() << "\n";
        goto fail;
    }
    
    return true;

fail:
    if (theGLContext) {
        SDL_GL_DeleteContext(theGLContext);
        theGLContext = nullptr;
    }
    if (theWindow) {
        SDL_DestroyWindow(theWindow);
        theWindow = nullptr;
    }
    SDL_Quit();
    return false;
}

void App::finish()
{
    if (theGLContext) {
        SDL_GL_DeleteContext(theGLContext);
        theGLContext = nullptr;
    }
    if (theWindow) {
        SDL_DestroyWindow(theWindow);
        theWindow = nullptr;
    }
    SDL_Quit();
}

void App::eventLoop()
{
    if (!redrawFunc)
        throw std::logic_error("App::eventLoop(): redrawFunc not set");

    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    return;
                    
                case SDL_MOUSEMOTION:
                    EventHandler::current()->onMouseMotion(event.motion);
                    break;
                    
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    EventHandler::current()->onMouseButton(event.button);
                    break;
                    
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    EventHandler::current()->onKeyboard(event.key);
                    break;

                default:
                    if (event.type == redrawTimerEventType) {
                        redrawFunc();
                        SDL_GL_SwapWindow(theWindow);
                    }
                    break;
            }
        }
        
        // EventHandler::current()->onIdle();
    }
}

void App::setRedrawFunc(void (*fn)())
{
    redrawFunc = fn;
}
