#ifndef ITMO_PARSAT_WORKERPOOL_H
#define ITMO_PARSAT_WORKERPOOL_H

#include <functional>
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>

#include "core/util/Generator.h"

namespace core::util {

template <typename WorkerType>
class WorkerPool {
 public:
  using task_t = std::function<void(WorkerType&)>;

 public:
  WorkerPool(uint32_t max_threads, std::function<WorkerType()> const& worker_generator) {
    for (uint32_t i = 0; i < max_threads; ++i) {
      _workers.push_back(worker_generator());
    }
    auto& parent_generator = core::Generator::current_thread_generator();
    for (uint32_t thread = 0; thread < max_threads; ++thread) {
      _threads.emplace_back([this, &parent_generator, thread] {
        core::Generator this_thread_generator(parent_generator);
        auto& worker = _workers[thread];
        while (!_stop) {
          std::unique_lock<std::mutex> ul(_mutex);
          _cv.wait(ul, [this] { return _stop || !_task_queue.empty(); });
          if (IPS_UNLIKELY(_stop)) {
            break;
          }
          if (IPS_UNLIKELY(_task_queue.empty())) {
            continue;
          }
          auto task = std::move(_task_queue.front());
          _task_queue.pop();
          ul.unlock();
          task(worker);
        }
      });
    }
  }

  ~WorkerPool() {
    {
      std::lock_guard<std::mutex> lg(_mutex);
      _stop = true;
    }
    _cv.notify_all();
    for (auto& thread : _threads) {
      if (thread.joinable()) {
        thread.join();
      }
    }
  }

  std::future<void> submit(task_t&& task) {
    auto packaged_task = std::packaged_task<void(WorkerType&)>(std::move(task));
    auto future = packaged_task.get_future();
    {
      std::lock_guard<std::mutex> lg(_mutex);
      _task_queue.emplace(std::move(packaged_task));
    }
    _cv.notify_one();
    return future;
  }

  [[nodiscard]] uint32_t max_threads() const noexcept {
    return _threads.size();
  }

  template <typename T>
  static void wait_for(std::vector<std::future<T>>& futures) {
    for (auto& future : futures) {
      future.get();
    }
  }

  std::vector<WorkerType>& get_workers() noexcept {
    return _workers;
  }

  std::vector<WorkerType> const& get_workers() const noexcept {
    return _workers;
  }

 private:
  bool _stop = false;
  std::mutex _mutex;
  std::condition_variable _cv;
  std::vector<std::thread> _threads;
  std::vector<WorkerType> _workers;
  std::queue<std::packaged_task<void(WorkerType&)>> _task_queue;
};

}  // namespace core::util

#endif  // ITMO_PARSAT_WORKERPOOL_H