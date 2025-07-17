#include <mutex>
#include <thread>
#include <vector>
#include <iostream>


std::mutex mtx;
size_t cnt = 0;

void worker(size_t iterations, size_t thread_id)
{
    for (size_t i = 0; i < iterations; ++i) {
        {
            std::lock_guard lock(mtx);
            ++cnt;
        }
        if (i % (iterations / 10) == 0)
            std::this_thread::yield();
    }
    std::cout << "Thread " << thread_id << " finished" << std::endl;
}

int main()
{
    constexpr size_t num_threads = 8;
    constexpr size_t iter_per_thread = 100000;

    std::vector<std::thread> pool;
    pool.reserve(num_threads);

    for (size_t t = 0; t < num_threads; ++t)
        pool.emplace_back(worker, iter_per_thread, t);

    for (auto& thread : pool)
        thread.join();

    std::cout << "Counter value: " << cnt << std::endl;

    return 0;
}