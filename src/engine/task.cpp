#include <cassert>
#include <queue>
#include <mutex>
#include <atomic>

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


    void awaitTask(Task task) {
        std::atomic<bool> done = false;
        addTask([task, &done]() {
            task();
            done = true;
            done.notify_one();
        });
        done.wait(false);
    }


    void execTasks() {
        std::lock_guard lock(mutex);
        while (!tasks.empty()) {
            tasks.front()();
            tasks.pop();
        }
    }

}
