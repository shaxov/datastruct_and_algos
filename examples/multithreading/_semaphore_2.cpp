#include <semaphore>      // std::counting_semaphore
#include <thread>         
#include <queue>
#include <mutex>
#include <iostream>
#include <chrono>
#include <random>

constexpr std::size_t BufferSize      = 5;      // вместимость буфера
constexpr std::size_t ItemsPerThread  = 20;     // сколько чисел создаёт каждый producer
constexpr std::size_t Producers       = 2;
constexpr std::size_t Consumers       = 2;

// Семафоры:  empty  — сколько свободных ячеек, full — сколько занято
std::counting_semaphore<BufferSize> empty(BufferSize);
std::counting_semaphore<BufferSize> full(0);

std::queue<int>   buffer;
std::mutex        buf_mutex;

// Генератор псевдослучайных чисел для "нагрузки"
int make_data()
{
    static thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution dist(1, 100);
    return dist(rng);
}

// ----- PRODUCER -------------------------------------------------------------
void producer(std::size_t id)
{
    for (std::size_t i = 0; i < ItemsPerThread; ++i) {
        int data = make_data();

        empty.acquire();                         // ждём свободную ячейку

        {   std::lock_guard lk(buf_mutex);
            buffer.push(data);                   // кладём данные в буфер
            std::cout << "[P" << id << "] + " << data
                      << "  (size=" << buffer.size() << ")\n";
        }

        full.release();                          // сигнал «в буфере есть элемент»
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

// ----- CONSUMER -------------------------------------------------------------
void consumer(std::size_t id)
{
    for (std::size_t i = 0; i < Producers * ItemsPerThread / Consumers; ++i) {
        full.acquire();                          // ждём, пока что-то появится

        int data;
        {   std::lock_guard lk(buf_mutex);
            data = buffer.front();
            buffer.pop();
            std::cout << "      [C" << id << "] − " << data
                      << "  (size=" << buffer.size() << ")\n";
        }

        empty.release();                         // сигнал «освобождена ячейка»
        std::this_thread::sleep_for(std::chrono::milliseconds(120));
    }
}

// ---------------------------------------------------------------------------
int main()
{
    std::vector<std::thread> threads;
    threads.reserve(Producers + Consumers);

    for (std::size_t p = 0; p < Producers; ++p)
        threads.emplace_back(producer, p);

    for (std::size_t c = 0; c < Consumers; ++c)
        threads.emplace_back(consumer, c);

    for (auto& thread : threads)
        thread.join();
}
