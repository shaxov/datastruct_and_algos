// g++ -std=c++20 -O2 -pthread mutex_demo.cpp && ./a.out
#include <iostream>
#include <vector>
#include <thread>      // std::jthread
#include <mutex>       // std::mutex, std::scoped_lock
#include <chrono>      // std::chrono::milliseconds

// Общий счётчик, разделяемый между потоками
std::size_t g_counter = 0;
std::mutex   g_mutex;          // единственный «замок» на счётчик

// Функция-работник: инкрементирует g_counter N раз
void worker(std::size_t iterations, std::size_t id)
{
    for (std::size_t i = 0; i < iterations; ++i) {
        // Критическая секция начинается
        {                           // отдельный блок — держим замок минимально долго
            std::scoped_lock lock(g_mutex);  // RAII: гарантированный unlock в конце блока
            ++g_counter;
        }   // <-- lock автоматически освобождается здесь

        // Делаем что-нибудь несинхронное (эмулируем работу)
        if (i % (iterations / 10) == 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "Thread " << id << " finished\n";
}

int main()
{
    constexpr std::size_t n_threads   = 8;
    constexpr std::size_t per_thread  = 100'000;

    {   // scope для вектора, чтобы потоки завершились до вывода результата
        std::vector<std::thread> pool;
        pool.reserve(n_threads);

        for (std::size_t t = 0; t < n_threads; ++t)
            pool.emplace_back(worker, per_thread, t);

        for (auto& thread : pool)
            thread.join();
    }

    std::cout << "Expected counter = " << n_threads * per_thread
              << "\nActual counter   = " << g_counter << '\n';
}
