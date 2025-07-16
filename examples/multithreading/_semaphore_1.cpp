#include <iostream>
#include <vector>
#include <thread>        
#include <semaphore>     // std::counting_semaphore
#include <chrono>

// Максимум 3 параллельных «соединения»
constexpr int MAX_CONNECTIONS = 3;
std::counting_semaphore<MAX_CONNECTIONS> db_slots(MAX_CONNECTIONS);

// RAII-обёртка: гарантированное release в деструкторе
class semaphore_guard {
public:
    explicit semaphore_guard(std::counting_semaphore<MAX_CONNECTIONS>& sem) : s(sem) {
        s.acquire();
    }
    ~semaphore_guard() { s.release(); }
    semaphore_guard(const semaphore_guard&)            = delete;
    semaphore_guard& operator=(const semaphore_guard&) = delete;
private:
    std::counting_semaphore<MAX_CONNECTIONS>& s;
};

void worker(int id)
{
    std::cout << "Task " << id << " ⇒ ожидает слот...\n";
    semaphore_guard guard(db_slots);          // блокируем, если слотов нет

    std::cout << "Task " << id << " ⇐ получил слот, работает\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));  // эмуляция I/O

    std::cout << "Task " << id << " ⇢ освобождает слот\n";
}

int main()
{
    std::vector<std::thread> pool;
    for (int i = 0; i < 10; ++i)
        pool.emplace_back(worker, i);

    for (auto& thread : pool)
        thread.join();
}
