#include "app.hpp"
#include "gl.hpp"

#include <iostream>
#include <stdexcept>

using namespace plush;

/////////////////////////////////////////////////////////////////////////////
// Redraw-related stuff
/////////////////////////////////////////////////////////////////////////////

/**
 * How often the timer requests a redraw (25 ms -> 40 frames per second).
 */
const Uint32 REDRAW_TIMER_INTERVAL = 25;

/**
 * Callback that is executed in the timer thread when the redraw timer fires.
 * Only adds an event to the event queue and returns; the redrawing is handled
 * in the main thread (App::eventLoop()).
 */
static Uint32 redrawTimerCallback(Uint32 interval, void *userData)
{
    RedrawTimerCallbackUserData *uData = static_cast<RedrawTimerCallbackUserData*>(userData);
    
    SDL_Event event;
    event.type = uData->eventTypeToSend;
    event.user.code = 0;
    event.user.data1 = nullptr;
    event.user.data2 = nullptr;
    
    SDL_PushEvent(&event);

    return interval;
}

/////////////////////////////////////////////////////////////////////////////
// App
/////////////////////////////////////////////////////////////////////////////

App::App()
    : m_ok(false), m_window(nullptr), m_GLContext(nullptr),
    m_redrawTimerEventID(static_cast<Uint32>(-1)),
    m_redrawTimer(0),
    m_redrawFunc(nullptr),
    m_keyFunc(nullptr),
    m_mouseMotionFunc(nullptr)
{
    GLenum err;
    
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << "\n";
        return;
    }

    m_window = SDL_CreateWindow("Electric Plush",
                                 
                                 // let the window manager place this window
                                 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                 
                                 1024, 768,
                                 SDL_WINDOW_OPENGL);
    if (!m_window) {
        std::cerr << "Error opening SDL window: " << SDL_GetError() << "\n";
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5); 
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    m_GLContext = SDL_GL_CreateContext(m_window);
    if (!m_GLContext) {
        std::cerr << "Error creating SDL GL context: " << SDL_GetError() << "\n";
        return;
    }

    SDL_GL_MakeCurrent(m_window, m_GLContext);

    // init crucial GL settings to sane values (some are not much sane by default)
    glEnable(GL_DEPTH_TEST);
    
    err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << "\n";
        return;
    }
    
    m_redrawTimerEventID = SDL_RegisterEvents(1);
    if (m_redrawTimerEventID == (Uint32)(-1)) {
        std::cerr << "Error: SDL_RegisterEvents(): " << SDL_GetError() << "\n";
        return;
    }
    
    m_redrawTimerCallbackUserData.eventTypeToSend = m_redrawTimerEventID;
    m_redrawTimer = SDL_AddTimer(REDRAW_TIMER_INTERVAL, redrawTimerCallback, &m_redrawTimerCallbackUserData);
    if (!m_redrawTimer) {
        std::cerr << "Error: SDL_AddTimer(): " << SDL_GetError() << "\n";
        return;
    }
    
    m_ok = true;
}

App::~App()
{
    if (m_GLContext)
        SDL_GL_DeleteContext(m_GLContext);

    if (m_window)
        SDL_DestroyWindow(m_window);

    SDL_Quit();
}

void App::eventLoop()
{
    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    return;
                    
                case SDL_MOUSEMOTION:
                    onMouseMotion(event.motion);
                    break;
                    
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    break;
                    
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    onKey(event.key);
                    break;

                default:
                    if (event.type == m_redrawTimerEventID) {
                        onRedraw();
                        SDL_GL_SwapWindow(m_window);
                    }
                    break;
            }
        }
    }
}

void App::onRedraw()
{
}

void App::onKey(const SDL_KeyboardEvent &event)
{
}

void App::onMouseMotion(const SDL_MouseMotionEvent &event)
{
}
