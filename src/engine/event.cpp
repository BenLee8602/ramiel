#include <cassert>
#include "event.h"

namespace ramiel {

    EventDispatcher::~EventDispatcher() {
        for (auto& h : handlers) {
            h->dispatcher = nullptr;
        }
    }

    void EventDispatcher::run() const {
        for (auto& h : handlers) {
            h->run();
        }
    }


    EventHandler::EventHandler(
        EventDispatcher* dispatcher,
        EventHandler::Fn handler
    )
        : dispatcher(dispatcher)
        , handler(handler)
    {
        assert(dispatcher);
        assert(handler);
        dispatcher->handlers.insert(this);
    }

    EventHandler::~EventHandler() {
        if (dispatcher) dispatcher->handlers.erase(this);
    }

    bool EventHandler::good() const {
        return dispatcher;
    }

    void EventHandler::run() const {
        assert(handler);
        handler();
    }

}
