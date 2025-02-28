#pragma once

#include <unordered_set>
#include <functional>

namespace ramiel {

    class EventHandler;

    class EventDispatcher {
    public:
        EventDispatcher() = default;
        ~EventDispatcher();

        void run() const;
    
    private:
        friend EventHandler;
        std::unordered_set<EventHandler*> handlers;
    };


    class EventHandler {
    public:
        using Fn = std::function<void()>;

        EventHandler(EventDispatcher* dispatcher, Fn handler);
        ~EventHandler();

        bool good() const;
        void run() const;

    private:
        friend EventDispatcher;
        std::function<void()> handler;
        EventDispatcher* dispatcher;
    };

}
