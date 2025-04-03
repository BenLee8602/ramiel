#include <cassert>
#include <queue>
#include <mutex>

#include "task.h"
using namespace ramiel;

namespace {

    std::queue<Task> tasks;
    std::mutex mutex;

}

namespace ramiel {

    void addTask(Task task) {
        assert(task);
        std::lock_guard lock(mutex);
        tasks.push(task);
    }

    void execTasks() {
        std::lock_guard lock(mutex);
        while (!tasks.empty()) {
            tasks.front()();
            tasks.pop();
        }
    }

}
