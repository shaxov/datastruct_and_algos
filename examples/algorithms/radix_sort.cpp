#include <vector>
#include <ranges>
#include <utility>
#include <iostream>


template<std::ranges::contiguous_range R>
requires std::unsigned_integral<std::ranges::range_value_t<R>>
constexpr void radixsort(R&& rng)
{
    using T = std::ranges::range_value_t<R>;
    constexpr size_t k_bytes = sizeof(T);
    constexpr size_t RADIX = 256;

    size_t n = static_cast<size_t>(std::ranges::distance(rng));
    if (n <= 1) return;

    std::vector<T> buf(n);

    auto* src = std::ranges::data(rng);
    auto* dst = buf.data();

    size_t cnt[RADIX];

    for (size_t byte = 0; byte < k_bytes; ++byte)
    {
        // 1. zeros counter
        std::fill(std::begin(cnt), std::end(cnt), 0);

        // 2. count occurences per buckets
        for (size_t i = 0; i < n; ++i) 
            ++cnt[(src[i] >> byte * 8) & 0xFF];

        // 3. exclusive prefix sum --> positions
        size_t sum = 0;
        for (size_t i = 0; i < RADIX; ++i)
        {
            auto tmp = cnt[i];
            cnt[i] = sum;
            sum += tmp;
        }

        // 4. stable scatten into dst
        for (size_t i = 0; i < n; ++i)
        {
            size_t bi = (src[i] >> byte * 8) & 0xFF; // bucket index
            dst[cnt[bi]++] = src[i];
        }

        // 5. swap with buffer
        std::swap(src, dst);
    }

    if (src != std::ranges::data(rng))
        std::copy(src, src + n, std::ranges::data(rng));
}


int main()
{
    std::vector<std::uint32_t> v{ 4294967295u, 23, 0, 777, 1024, 1, 42 };
    radixsort(v);

    for (auto x : v) std::cout << x << ' ';         // 0 1 23 42 777 1024 4294967295
}