#include <ranges>
#include <iostream>

template<std::ranges::random_access_range R,
         class Compare = std::less<>>
constexpr void insertion_sort(R&& rng, Compare comp = {}) {
    using std::ranges::begin;
    using std::ranges::end;

    if (begin(rng) == end(rng)) return;

    auto it = begin(rng) + 1;

    while (it != end(rng)) {
        auto key = std::move(*it);
        auto hole = it;

        while (hole > begin(rng) && comp(key, *(hole - 1))) {
            *hole = std::move(*(hole - 1));
            --hole;
        }
        *hole = std::move(key);
        ++it;
    }
}

int main()
{
    std::vector<int> v{ 5, 1, 4, 2, 8, 0, 2 };

    insertion_sort(v);
    for (int x : v) std::cout << x << ' ';
    std::cout << std::endl;

    insertion_sort(v, std::greater<>{});
    for (int x : v) std::cout << x << ' ';
}