#include <ramiel/test.h>
#include <ramiel/engine.h>
using namespace ramiel;

RAMIEL_TEST_ADD(Event) {
    uint32_t count = 0;
    EventHandler::Fn incrementCount = [&count]() { count++; };

    EventDispatcher* dispatch = new EventDispatcher();
    EventHandler handler1(dispatch, incrementCount);
    dispatch->run();
    RAMIEL_TEST_ASSERT(count == 1);
    
    {
        EventHandler handler2(dispatch, incrementCount);
        dispatch->run();
        RAMIEL_TEST_ASSERT(count == 3);
    }

    dispatch->run();
    RAMIEL_TEST_ASSERT(count == 4);

    RAMIEL_TEST_ASSERT(handler1.good());
    delete dispatch;
    RAMIEL_TEST_ASSERT(!handler1.good());
}
