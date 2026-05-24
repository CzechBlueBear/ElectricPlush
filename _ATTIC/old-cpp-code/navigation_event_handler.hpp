#ifndef NAVIGATION_EVENT_HANDLER_HPP
#define NAVIGATION_EVENT_HANDLER_HPP

#include "event_handler.hpp"

namespace plush {
 
    /// Event handler to be used when navigating the scene.
    class NavigationEventHandler : public EventHandler {
    public:
        
        void onIdle();
    };
}

#endif