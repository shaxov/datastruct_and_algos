#include <thread>
#include <iostream>
#include <semaphore>


template<size_t N>
void worker(size_t task_id, std::counting_semaphore<N>& slots)
{
    std::cout << "Task " << task_id << " is waiting for slot" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    slots.acquire();

    std::cout << "Task " << task_id << " recieved slot" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    slots.release();
    std::cout << "Task " << task_id << " released slot" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

int main()
{
    constexpr size_t max_connections = 3;
    std::counting_semaphore<max_connections> slots(max_connections);

    constexpr size_t num_threads = 8;
    std::vector<std::thread> pool;
    pool.reserve(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        pool.emplace_back(worker<max_connections>, i, std::ref(slots));
    }

    for (auto& t : pool)
        t.join();

    return 0;
}