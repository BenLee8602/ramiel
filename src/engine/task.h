#pragma once

#include <functional>

namespace ramiel {

    using Task = std::function<void()>;

    void addTask(Task task);
    void execTasks();

}
